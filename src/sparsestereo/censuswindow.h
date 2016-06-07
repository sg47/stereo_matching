/*
 * Author: Konstantin Schauwecker
 * Year:   2012
 */

#ifndef SPARSESTEREO_CENSUSWINDOW_H
#define SPARSESTEREO_CENSUSWINDOW_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "hammingdistance.h"
#include "simd.h"
#include <math.h>       /* exp */

//#define SPARSESTEREO_NO_POPCNT //Defined by CMake

#if defined(SPARSESTEREO_NO_POPCNT)
#define SPARSESTEREO_SSE_OPTIMIZE
#endif
using namespace cv;
namespace sparsestereo 
{

	// Matches over a set of census windows
	template <int SIZE>
	class CensusWindow 
	{
		public:
		CensusWindow() 
		{
			#ifdef SPARSESTEREO_SSE_OPTIMIZE
			lookupTablePtr = hammingDist.getLookupTable();
			lookupTableVec = SIMD::scalar4((int)hammingDist.getLookupTable());
			zero = SIMD::scalar16(0);
			#endif

		}
		
		void setReferenceImage(const cv::Mat_<unsigned int>& image, Mat& colorImg) 
		{
			refImage = image;
			cvtColor(colorImg, refColorImage, CV_BGR2Lab);

		}
		
		void setComparisonImage(const cv::Mat_<unsigned int>& image, Mat& colorImg) 
		{
			compImage = image;
			cvtColor(colorImg, compColorImage, CV_BGR2Lab);

		}
		
		

		
		const cv::Mat_<unsigned int>& getReferenceImage() const {return refImage;}
		const cv::Mat_<unsigned int>& getComparisonImage() const {return compImage;}
		const int getWindowSize() const {return SIZE;}
		
		// Sets the position of the reference window
		void setReferencePoint(const cv::Point2i& point);
		
		// Performs a window matching using census transformed images
		__always_inline short match(cv::Point2i point, int combination) const;
		
		private:
		v4si lookupTableVec;
		v4si refWindow[6];
		v16qi zero;
	
		const unsigned char* lookupTablePtr;
		HammingDistance hammingDist;
		cv::Mat_<unsigned int> refImage;
		cv::Mat_<unsigned int> compImage;
		cv::Mat refColorImage;
		cv::Mat compColorImage;
		cv::Point2i refPoint;

		// Stores a window in two SSE vectors
		void loadWindow(const cv::Mat_<unsigned int>& image, const cv::Point2i& point, v4si* dstWindow) const;
//		float weightAggregation(Mat leftLab,Mat rightLab,int Yc,int Yp,int k,int pointY,int pointX, int pointY1,int pointX1)const;
	} 

	__attribute__ ((aligned (16)));
	

	template <int SIZE>
	void CensusWindow<SIZE>::loadWindow(const cv::Mat_<unsigned int>& image, const cv::Point2i& point, v4si* dstWindow) const {}
	

	#ifdef SPARSESTEREO_SSE_OPTIMIZE
	template <>
	void CensusWindow<5>::loadWindow(const cv::Mat_<unsigned int>& image, const cv::Point2i& point, v4si* dstWindow) const 
	{
		dstWindow[0] = (v4si)__builtin_ia32_loaddqu((char*)&image(point.y-2, point.x-2));
		dstWindow[1] = (v4si)__builtin_ia32_loaddqu((char*)&image(point.y-1, point.x-2));
		dstWindow[2] = (v4si)__builtin_ia32_loaddqu((char*)&image(point.y, point.x-2));
		dstWindow[3] = (v4si)__builtin_ia32_loaddqu((char*)&image(point.y+1, point.x-2));
		dstWindow[4] = (v4si)__builtin_ia32_loaddqu((char*)&image(point.y+2, point.x-2));
		
		// Unfortunately, the rest cannot be loaded aligned
		v4si buffer = {image(point.y-2, point.x+2), image(point.y-1, point.x+2), image(point.y, point.x+2), image(point.y+1, point.x+2)};
		dstWindow[5] = buffer;
	}
#endif
	
	template <int SIZE>
	void CensusWindow<SIZE>::setReferencePoint(const cv::Point2i& point) 
	{
		refPoint = point;
	}
	
#ifdef SPARSESTEREO_SSE_OPTIMIZE
	template <>
	void CensusWindow<5>::setReferencePoint(const cv::Point2i& point) {
		loadWindow(refImage, point, refWindow);
	}
#endif
	
	template <int SIZE>
	__always_inline short CensusWindow<SIZE>::match(cv::Point2i point,int combination) const {
		long double costs = 0;
		float weightsAgg=0;
#ifndef SPARSESTEREO_NO_POPCNT
								//	cout<<"censuswindow.h Line 101"<<endl;

		// for(int y=-SIZE/2; y<=SIZE/2; y++) {
		// 	// int y1 = refPoint.y+y ; 
		// 	// if(y1<0)
		// 	// 	y1 = 0; 
		// 	// int x1 = refPoint.x -SIZE/2 ;
		// 	// if(x1<0)
		// 	// 	x1= 0; 

		// 	// int x2 = point.x -SIZE/2 ;
		// 	// if(x2<0)
		// 	// 	x2 = 0 ;

		// 	// int y2 = point.y + y; 

		// 	// if(y2<0)
		// 	// 	y2=0;
		// 	unsigned long long* ptr1 = (unsigned long long*)&refImage(y1, x1);
		// 								//	cout<<"censuswindow.h Line 105"<<endl;

		// 	unsigned long long* ptr2 = (unsigned long long*)&compImage(y2, x2);
		// 										//cout<<"censuswindow.h Line 108"<<endl;

		// 	for(int x=0; x<=SIZE/2;x++)
		// 		{
		// 			//cout<<"censuswindow.h Line 112"<<endl;
		// 			//cout<<" 1"<<ptr1[x]<<endl; 
		// 			//if(ptr1[x]!= NULL && ptr2[x]!= NULL)
		// 			costs += __builtin_popcountll(ptr1[x] ^ ptr2[x]);
		// 				//				cout<<"censuswindow.h Line 113"<<endl;

		// 		}
		// }
		//cout<<"censuswindow.h Line 159"<<endl;
		// Mat leftLab, rightLab ; 
		// cvtColor(colorLeft, leftLab, CV_BGR2Lab);
  //  		cvtColor(colorRight, rightLab, CV_BGR2Lab);
		//cout<<"censuswindow.h Line 163"<<endl;
		if(combination == 3)
		{
	   		int Yc =32 , Yp = 36 ,k=13; 	
			for(int y=-SIZE/2; y<=SIZE/2; y++)
				for(int x=-SIZE/2; x<=SIZE/2; x++)
					{
						// cout<<"Value1 = "<<refPoint.y + y<<endl;
								// cout<<"Value2 = "<<refPoint.x + x<<endl;
						if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
						{
							//cout<<"censuswindow.h Line 173"<<endl;
							float colorDiffLeft = pow( pow((short)refColorImage.at<cv::Vec3b>(refPoint.y ,refPoint.x)[0] - (short)refColorImage.at<cv::Vec3b>(refPoint.y + y,refPoint.x + x)[0],2)
											+ pow((short)refColorImage.at<cv::Vec3b>(refPoint.y ,refPoint.x )[1] - (short)refColorImage.at<cv::Vec3b>(refPoint.y + y,refPoint.x + x)[1],2) 
											+ pow((short)refColorImage.at<cv::Vec3b>(refPoint.y,refPoint.x)[2] - (short)refColorImage.at<cv::Vec3b>(refPoint.y + y,refPoint.x + x)[2],2),0.5);
							float spatialDiffLeft = pow(pow(y,2)+pow(x,2),0.5);
							float weightLeft =  k*exp(-((colorDiffLeft/Yc)+(spatialDiffLeft/Yp)));

							float colorDiffRight = pow( pow((short)compColorImage.at<cv::Vec3b>(point.y ,point.x)[0] - (short)compColorImage.at<cv::Vec3b>(point.y + y,point.x + x)[0],2)
											+ pow((short)compColorImage.at<cv::Vec3b>(point.y ,point.x )[1] - (short)compColorImage.at<cv::Vec3b>(point.y + y,point.x + x)[1],2) 
											+ pow((short)compColorImage.at<cv::Vec3b>(point.y,point.x)[2] - (short)compColorImage.at<cv::Vec3b>(point.y + y,point.x + x)[2],2),0.5);
							float spatialDiffRight = pow(pow(y,2)+pow(x,2),0.5);
							float weightRight =  k*exp(-((colorDiffRight/Yc)+(spatialDiffRight/Yp)));
							// cout<<"Color Diff = " <<colorDiff<<endl;
							// cout<<"Spatial Diff = " <<spatialDiff<<endl;
							//  cout<<"weight = " <<1-weight<<endl;
							//  cout<<"H distance = " <<(unsigned int)hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x))<<endl;
							//   cout<<"cost = " <<(1-weight)*hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x))<<endl;
							
							//float weight = 1;
							costs += weightLeft*weightRight*(16-hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x)));
							weightsAgg += weightLeft*weightRight;
						}
								//cout<<"censuswindow.h Line 182"<<endl;

					}
					//cout<<"Costs = "<<costs<<endl;
					costs = -costs/weightsAgg ; 
		}
		else if(combination ==1)
		{
				for(int y=-SIZE/2; y<=SIZE/2; y++)
					for(int x=-SIZE/2; x<=SIZE/2; x++)
						{
							if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
								costs += hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x));
		
						}
		}
		else if(combination==0)
		{
			//cout<<"Here I am"<<endl;
			int VerticalBlockSize= 61 ;
			int HorizontalblockSize=61 ; 
			int squareBlockSize1 = 11 ; 
			int squareBlockSize2 = 3 ;
			long double costVertical=0 ,costHorizontal=0,costBlockSize1 = 0, costBlockSize2 =0 ; 
			for(int y=-VerticalBlockSize/2 ; y<VerticalBlockSize/2 ;y++)
			{	int x =0  ;
				if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
				{
					costVertical += (hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x)));
				}
			}



			for(int x=-HorizontalblockSize/2 ; x<HorizontalblockSize/2 ;x++)
			{	int y =0  ;
				if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
				{
					costHorizontal += (hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x)));
				}
			}

			for(int y=-squareBlockSize1/2; y<=squareBlockSize1/2; y++)
				for(int x=-squareBlockSize1/2; x<=squareBlockSize1/2; x++)
				{
					if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
					{
						costBlockSize1 += (hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x)));
					}
				}

			for(int y=-squareBlockSize2/2; y<=squareBlockSize2/2; y++)
				for(int x=-squareBlockSize2/2; x<=squareBlockSize2/2; x++)
				{
					if(!((refPoint.y +y <1)||(refPoint.x + x <0)))
					{
						costBlockSize2 += (hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),compImage(point.y + y, point.x + x)));
					}
				}

				costs = costBlockSize2*costBlockSize1*costHorizontal*costVertical/10000000;
			//	cout<<"Costs ="<<costs<<endl;
		}

#else
		for(int y=-SIZE/2; y<=SIZE/2; y++)
			for(int x=-SIZE/2; x<=SIZE/2; x++)
				costs += hammingDist.calculate(refImage(refPoint.y + y, refPoint.x + x),
					compImage(point.y + y, point.x + x));
#endif
		
		return costs;
	}
	
#ifdef SPARSESTEREO_SSE_OPTIMIZE
#ifdef __LP64__
	// SSE2 optimized implementation for 64-bit systems.
	template <>
	__always_inline short CensusWindow<5>::match(cv::Point2i point) const {
		v8hi xorRes;
		unsigned int sum;
		
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y-2, point.x-2)), (v2di)refWindow[0]);
		sum = lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
			
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y-1, point.x-2)), (v2di)refWindow[1]);
		sum += lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
		
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y, point.x-2)), (v2di)refWindow[2]);
		sum += lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
		
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y+1, point.x-2)), (v2di)refWindow[3]);
		sum += lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
		
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y+2, point.x-2)), (v2di)refWindow[4]);
		sum += lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
		
		v4si buffer1 = {compImage(point.y-2, point.x+2), compImage(point.y-1, point.x+2), compImage(point.y, point.x+2), compImage(point.y+1, point.x+2)};
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)buffer1, (v2di)refWindow[5]);
		sum += lookupTableVec[(unsigned short)SIMD::element8(xorRes, 0)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 1)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 2)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 3)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 4)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 5)]
			+ lookupTableVec[(unsigned short)SIMD::element8(xorRes, 6)] + lookupTableVec[(unsigned short)SIMD::element8(xorRes, 7)];
		
		unsigned short lastXor = compImage(point.y+2, point.x+2) ^ refImage(refPoint.y + 2, refPoint.x+2);
		sum += lookupTablePtr[(unsigned short)lastXor] + lookupTablePtr[((unsigned short*)&lastXor)[1]];
		
		return sum;
	}

#else

	// SSE2 optimized implementation for 32-bit systems.
	template <>
	__always_inline short CensusWindow<5>::match(cv::Point2i point) const {
		
		v8hi xorRes;
		v4si lookupPtr;
		unsigned int sum;
		
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y-2, point.x-2)), (v2di)refWindow[0]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum = *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
			
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y-1, point.x-2)), (v2di)refWindow[1]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
			
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y, point.x-2)), (v2di)refWindow[2]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
			
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y+1, point.x-2)), (v2di)refWindow[3]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
			
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)__builtin_ia32_loaddqu((char*)&compImage(point.y+2, point.x-2)), (v2di)refWindow[4]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
			
		v4si buffer1 = {compImage(point.y-2, point.x+2), compImage(point.y-1, point.x+2), compImage(point.y, point.x+2), compImage(point.y+1, point.x+2)};
		xorRes = (v8hi)__builtin_ia32_pxor128((v2di)buffer1, (v2di)refWindow[5]);
		lookupPtr = (v4si)__builtin_ia32_punpcklwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		lookupPtr = (v4si)__builtin_ia32_punpckhwd128(xorRes, (v8hi)zero) + lookupTableVec;
		sum += *((unsigned char*)SIMD::element4(lookupPtr, 0)) + *((unsigned char*)SIMD::element4(lookupPtr, 1))
			+ *((unsigned char*)SIMD::element4(lookupPtr, 2)) + *((unsigned char*)SIMD::element4(lookupPtr, 3)); 
		
		unsigned short lastXor = compImage(point.y+2, point.x+2) ^ refImage(refPoint.y + 2, refPoint.x+2);
		sum += lookupTablePtr[(unsigned short)lastXor] + lookupTablePtr[((unsigned short*)&lastXor)[1]];
		
		return sum;
	}
#endif
#endif
}

#endif
