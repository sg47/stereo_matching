/*
 * Author: Konstantin Schauwecker
 * Year:   2012
 */

#ifndef SPARSESTEREO_SPARSESTEREO_INL_H
#define SPARSESTEREO_SPARSESTEREO_INL_H

#include "sparsestereo.h"
#include <boost/scoped_array.hpp>
#include <limits>
#include <algorithm>
#include "exception.h"
#include "stereorectification.h"
#include "simd.h"

namespace sparsestereo {
	using namespace std;

	template <class CORRELATION, typename COST_TYPE>
	SparseStereo<CORRELATION, COST_TYPE>::SparseStereo(int maxDisparity,  float yTolerance, float uniqueness, 
		StereoRectification* rect, bool subpixelFeatures, bool storeUnmatched, int leftRightStep)
		: maxDisparity(maxDisparity), yTolerance(yTolerance), uniqueness(uniqueness), rect(rect),
		storeUnmatched(storeUnmatched), leftRightStep(leftRightStep), precompEpilinesStart(0, 0),
		sparseRect(subpixelFeatures, rect) {
	}
	
	template <class CORRELATION, typename COST_TYPE>
	SparseStereo<CORRELATION, COST_TYPE>::~SparseStereo() {
	}
	
	template <class CORRELATION, typename COST_TYPE>
	void SparseStereo<CORRELATION, COST_TYPE>::match(const cv::Mat& left, const cv::Mat& right,
		const std::vector<cv::KeyPoint>& leftFeat, const std::vector<cv::KeyPoint>& rightFeat,
		std::vector<SparseMatch>* matches , cv::Mat& colorLeft, cv::Mat& colorRight ,int combination) {
			//cout<<"Sparsestereo-inl.h Line 36"<<endl;

		if(left.size() != right.size() || (rect != NULL && left.size() != rect->getCalibrationResult().imageSize))
			throw Exception("Mismatching image sizes");
		if(leftFeat.size() == 0 || rightFeat.size() == 0)
			return; // Can't match anything
		
		// For the first time, or when the image size changes, compute epiline lookup table
		if(left.cols != precompEpilinesStart.cols || left.rows != precompEpilinesStart.rows)
			sparseRect.precomputeEpilinesStart(left.cols, left.rows, &precompEpilinesStart);
				//	//cout<<"Sparsestereo-inl.h Line 46"<<endl;

		// Rectify feature points
		sparseRect.rectify(leftFeat, rightFeat, &leftFeatures, &rightFeatures);	//sort and convert features into a specific datastructure
	
		// Features are now sorted from top left to bottom right
		boost::shared_array<unsigned int> offsets = SIMD::alignedNew<unsigned int>(left.rows);
		int maxRowLen __attribute__((unused)) = getRowOffsets(rightFeatures, offsets.get(), left.rows);
					////cout<<"Sparsestereo-inl.h Line 54"<<endl;
		minimumMatches.clear();
		minimumMatches.resize(leftFeatures.size());
		//	cout<<"Sparsestereo-inl.h Line 57"<<endl;

		// Perform matching
		calcCosts(left, right, offsets.get(),colorLeft, colorRight,combination);
	//		cout<<"Sparsestereo-inl.h Line 61"<<endl;
		// Perform left/right consistency check
		denseConsistencyCheck(left, right, colorLeft, colorRight,combination);
				//	cout<<"Sparsestereo-inl.h Line 64"<<endl;

		// Compose sparse disparity list
	//	cout<<"left features size = "<<(int) leftFeatures.size()<<endl;
		//int count =0;
		for(int i=0; i<(int)leftFeatures.size(); i++) {
			const cv::KeyPoint* rightFeature = NULL;
			cv::Point2f rightRect = leftFeatures[i].rectPoint + cv::Point2f(1, 0); //Disparity -1
			short cost = 0;
			
			if(minimumMatches[i].first != -1) {
				rightFeature = rightFeatures[minimumMatches[i].first].imgPoint;
				rightRect = rightFeatures[minimumMatches[i].first].rectPoint;
				cost = minimumMatches[i].second;
				//count++ ;
			}

						

			if(rightFeature != NULL || storeUnmatched)
			{
				matches->push_back(SparseMatch(leftFeatures[i].imgPoint, rightFeature, leftFeatures[i].rectPoint, rightRect, cost));
			//	cout<<"Points = "<<leftFeatures[i]->imgPoint.x<<endl;
			}
		}
	//	cout<<"Count ="<<count<<endl;
	//	cout<<"Sparsestereo-inl.h Line 82"<<endl;
	}
	
	template <class CORRELATION, typename COST_TYPE>
	void SparseStereo<CORRELATION, COST_TYPE>::calcCosts(const cv::Mat& left, const cv::Mat& right, unsigned int* rowOffsets,cv::Mat& colorLeft, cv::Mat& colorRight, int combination) {
		
		int lastRow = -1e9; //Invalid value
		CORRELATION correlation;
	//	//cout<<"Sparsestereo-inl.h Line 90"<<endl;

		correlation.setReferenceImage(left,colorLeft);
		correlation.setComparisonImage(right,colorRight);
		//cout<<"Sparsestereo-inl.h Line 92"<<endl;

		for(int l=0; l<(int)leftFeatures.size(); l++) {
			// Find row start and end points
			int ly = (int)(leftFeatures[l].rectPoint.y + 0.5);
			int rightStart = rowOffsets[min(left.rows-1, max(0, int(ly - yTolerance)))];
			int rightEnd = rowOffsets[min(left.rows-1, max(0, int(ly + yTolerance) + 2))];
		//cout<<"Sparsestereo-inl.h Line 99"<<endl;

			if(ly != lastRow) {
				// Skip top and bottom
				if(ly < correlation.getWindowSize()/2) {
					continue;
				}
				else if(ly >= left.rows - correlation.getWindowSize()/2 -1) {
					// We reached the bottom of the image. Lets invalidate the remaining features.
					for(; l<(int)leftFeatures.size(); l++)
						minimumMatches[l].first = -1;
					break;
				}
				
				// We have to initialize a new cost cube row
				lastRow = ly;
			}		
			//cout<<"Sparsestereo-inl.h Line 116"<<endl;

			COST_TYPE minCost = numeric_limits<COST_TYPE>::max();
		//	COST_TYPE minCost = -1;

			//cout<<"Min Cost = " << minCost <<endl;
			int minRightFeature = -1;
			
			correlation.setReferencePoint(cv::Point2i(int(leftFeatures[l].imgPoint->pt.x + 0.5),
				int(leftFeatures[l].imgPoint->pt.y + 0.5)));
						//	//cout<<"Sparsestereo-inl.h Line 124"<<endl;

			for(int r=rightStart; r<rightEnd; r++) {
				// First test if this would be a valid match
				if(	rightFeatures[r].rectPoint.x <= leftFeatures[l].rectPoint.x &&
					rightFeatures[r].rectPoint.x >= leftFeatures[l].rectPoint.x - maxDisparity &&
					fabs(leftFeatures[l].rectPoint.y - rightFeatures[r].rectPoint.y) <= yTolerance &&
					rightFeatures[r].rectPoint.x >= correlation.getWindowSize()/2 &&
					rightFeatures[r].rectPoint.x < left.cols - correlation.getWindowSize()/2)
				{		//	//cout<<"Sparsestereo-inl.h Line 132"<<endl;

					// It is! Let's compute a cost
					COST_TYPE currentCost = correlation.match(
						cv::Point2i(int(rightFeatures[r].imgPoint->pt.x + 0.5), int(rightFeatures[r].imgPoint->pt.y + 0.5)) ,combination);
						//	//cout<<"Sparsestereo-inl.h Line 138"<<endl;

					if(currentCost < minCost) { // Only store smaller costs
						minCost = currentCost;
						minRightFeature = r;
					}
										//		//cout<<"Sparsestereo-inl.h Line 144"<<endl;

				}
			}
						//	//cout<<"Sparsestereo-inl.h Line 143"<<endl;


			minimumMatches[l] = std::pair<int, COST_TYPE>(minRightFeature, minCost);
		}
			//	//cout<<"Sparsestereo-inl.h Line 143"<<endl;

	}
	
	template <class CORRELATION, typename COST_TYPE>
	int SparseStereo<CORRELATION, COST_TYPE>::getRowOffsets(const std::vector<SparseRectification::RectifiedFeature>& features,
		unsigned int* offsets, int maxRows) {
		int lastRow=-1, lastOffset = 0, longestRowLen = -1;
		
		for(unsigned int i=0; i<features.size(); i++) {
			int y = (int)(features[i].rectPoint.y + 0.5);
			if(y >= maxRows -1)
				break; // The rectified point is outside the visible image
			else if(y<0)
				continue;
				
			if(y != lastRow) {
				// Save offset
				for(int j=lastRow+1; j<=y; j++)
					offsets[j] = i;
					
				if((int)i - lastOffset > longestRowLen)
					longestRowLen = i - lastOffset;
					
				lastRow = y;
				lastOffset = i;
			}
		}
		
		// Save offset for the remaining items
		for(int i=lastRow+1; i < maxRows; i++)
			offsets[i] = features.size();
		if((int)features.size() - lastOffset > longestRowLen)
			longestRowLen = features.size() - lastOffset;
			
		return longestRowLen; //Return the maximum row length
	}

	template <class CORRELATION, typename COST_TYPE>
	void SparseStereo<CORRELATION, COST_TYPE>::denseConsistencyCheck(const cv::Mat& left, const cv::Mat& right ,cv::Mat& colorLeft, cv::Mat& colorRight, int combination) {			
		int lastRow = -1e9; //Invalid value
		CORRELATION correlation;
		correlation.setReferenceImage(right,colorRight);
		correlation.setComparisonImage(left,colorLeft);
	//	//cout<<"Sparsestereo-inl.h Line 196"<<endl;
		for(int l = 0; l < (int)leftFeatures.size(); l++) {
			int ly = (int)(leftFeatures[l].rectPoint.y + 0.5);
			if(ly != lastRow) {		
				// Skip top and bottom
				if(ly < correlation.getWindowSize()/2)
					continue;
				else if(ly >= left.rows - correlation.getWindowSize()/2 -1)
					break;
								
				lastRow = ly;
			}
			////cout<<"Sparsestereo-inl.h Line 208"<<endl;
			// Get the minimum match and cost
			int r = minimumMatches[l].first;
			COST_TYPE minCost = minimumMatches[l].second;
			
			if(r == -1)
				continue;
			////cout<<"Sparsestereo-inl.h Line 215"<<endl;
			// Get epiline start and end
			float leftStartX = precompEpilinesStart(leftFeatures[l].imgPoint->pt.y, max(0, min(
				precompEpilinesStart.cols, (int)(rightFeatures[r].rectPoint.x+0.5))));
			int startX = max(correlation.getWindowSize()/2, int(leftStartX /*+ 0.5*/));
			int endX = min(precompEpilinesStart.cols - correlation.getWindowSize()/2,
				int(leftStartX + maxDisparity /*+ 0.5*/));
			Epiline epiline = rect!=NULL ? rect->getLeftEpiline(leftFeatures[l].imgPoint->pt) : Epiline(leftFeatures[l].imgPoint->pt.y);
			if(!epiline.isValid())
				continue;
						////cout<<"Sparsestereo-inl.h Line 225"<<endl;

			// Preform consistency check
			correlation.setReferencePoint(cv::Point2i(int(rightFeatures[r].imgPoint->pt.x + 0.5), int(rightFeatures[r].imgPoint->pt.y + 0.5)));
						// //cout<<"Sparsestereo-inl.h Line 229"<<endl;
						// cout<<"Startx ="<<startX <<endl; 
						// cout<<"enx ="<<endX <<endl; 
						// cout<<"leftRightStep ="<<leftRightStep <<endl; 


			for(int x = startX; x < endX; x+= leftRightStep) {
				int y = (int)(epiline.at(x) + 0.5);
				if(y < correlation.getWindowSize()/2 || y >= left.rows - correlation.getWindowSize()/2)
					continue;
							////cout<<"Sparsestereo-inl.h Line 235"<<endl;

				COST_TYPE currentCost = correlation.match(cv::Point2i(x, y),combination);
							////cout<<"Sparsestereo-inl.h Line 238"<<endl;

				if(currentCost < minCost/uniqueness && fabs(x - int(leftFeatures[l].imgPoint->pt.x+0.5)) > leftRightStep) {
					minimumMatches[l].first = -1;
					break;
				}
			}
		}
	}	
}

#endif
