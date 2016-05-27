/*
 * Author: Konstantin Schauwecker
 * Year:   2012
 */

#ifndef SPARSESTEREO_CENSUS_INL_H
#define SPARSESTEREO_CENSUS_INL_H

#include "exception.h"
#include "census.h"

namespace sparsestereo {
	template<typename T>
	void Census::transform5x5(const cv::Mat_<T>& input, cv::Mat_<unsigned int>* output, int edgeNo) {
		int maxX=input.cols-2, maxY=input.rows-2;

		for(int y=2; y<maxY; y++)
			for(int x=2; x<maxX; x++) {
				T centoid = input(y,x);
				switch(edgeNo)
				{
					case 1: (*output)(y,x) = ((input(y, x-1) > input(y, x+1)) ) ; 
							break;

					case 2: (*output)(y,x) =
							 ((input(y-1, x) > input(y+1, x)) <<1) |
							 ((input(y, x-1) > input(y, x+1)) ) ; 


					case 4: (*output)(y,x) =
							((input(y+1, x+1) > input(y-1, x-1)) << 3) |
							((input(y, x-1) > input(y, x+1)) << 2)	|
							((input(y-1, x+1) > input(y+1, x-1)) << 1) |
							((input(y-1, x) > input(y+1, x)) )	;
							break;

					case 8:  (*output)(y,x) =
							 ((input(y+2, x-2) > input(y-2, x+2)) <<7) |
 							 ((input(y-2, x-2) > input(y+2, x+2)) <<6) |
 							 ((input(y, x+2) > input(y, x-2)) <<5) |
 							 ((input(y+2, x) > input(y-2, x)) <<4) |
 							 ((input(y+1, x+1) > input(y-1, x-1)) <<3) |
 							 ((input(y-1, x+1) > input(y+1, x-1)) <<2) |
 							 ((input(y-1, x) > input(y+1, x)) <<1) <<1|
 							  ((input(y, x-1) > input(y, x+1)) <<1) ;



					case 12 :   (*output)(y,x) =
								((input(y-2, x-2) > input(y+2, x+2)) << 11) |
								((input(y, x+2) > input(y, x-2)) << 10) |
								((input(y+2, x-2) > input(y-2, x+2)) << 9) |
								((input(y+2, x) > input(y-2, x)) << 8) |
								((input(y+1, x+1) > input(y-1, x-1)) << 7) |
								((input(y+1, x+1) > input(y, x-1)) << 6) |
								((input(y+1, x-1) > input(y, x+1)) << 5) |
								((input(y-1, x-1) > input(y, x+1)) << 4) |
								((input(y-1, x+1) > input(y+1, x-1)) << 3) |
								((input(y-1, x+1) > input(y, x-1)) << 2) |
								((input(y-1, x) > input(y+1, x)) << 1) |
								((input(y, x-1) > input(y, x+1)) ) ;
								break;

					case 16 :    (*output)(y,x) =
								((input(y-2, x-2) > input(y+2, x+2)) << 15) |
								((input(y+1, x+2) > input(y-1, x-2)) << 14) |
								((input(y, x+2) > input(y, x-2)) << 13) |
								((input(y+1, x-2) > input(y-1, x+2)) << 12) |
								((input(y+2, x-2) > input(y-2, x+2)) << 11) |
								((input(y-2, x+1) > input(y+2, x-1)) << 10) |
								((input(y+2, x) > input(y-2, x)) << 9) |
								((input(y+2, x+1) > input(y-2, x-1)) << 8) |
								((input(y-1, x) > input(y+1, x)) << 7) |
								((input(y+1, x+1) > input(y-1, x-1)) << 6) |
								((input(y+1, x+1) > input(y, x-1)) << 5) |
								((input(y-1, x-1) > input(y, x+1)) << 4) |
								((input(y+1, x-1) > input(y, x+1)) << 3) |
								((input(y-1, x) > input(y+1, x)) << 2) |
								((input(y-1, x+1) > input(y+1, x-1)) << 1) |
								((input(y-1, x+1) > input(y, x-1)) ) ;

				}
					// Row 1	//
					//((centoid > input(y-2, x-2)) << 24) |
					// ((centoid > input(y-2, x-1)) << 23) |
					// ((centoid > input(y-2, x)) << 22) |
					// ((centoid > input(y-2, x+1)) << 21) |
					// ((centoid > input(y-2, x+2)) << 20) |
					// // Row 2
					// ((centoid > input(y-1, x-2)) << 19) |
					// ((centoid > input(y-1, x-1)) << 18) |
					// ((centoid > input(y-1, x)) << 17) |
					// ((centoid > input(y-1, x+1)) << 16) |
					// ((centoid > input(y-1, x+2)) <<15) |
					// //Row 3
					// ((centoid > input(y, x-2)) << 14) |
					// ((centoid > input(y, x-1)) << 13) |
					// ((centoid > input(y, x)) << 12) |
					// ((centoid > input(y, x+1)) << 11) |
					// ((centoid > input(y, x+2)) << 10) |
					// // Row 4
					// ((centoid > input(y+1, x-2)) << 9) |
					// ((centoid > input(y+1, x-1)) << 8) |
					// ((centoid > input(y+1, x)) << 7) |
					// ((centoid > input(y+1, x+1)) << 6) |
					// ((centoid > input(y+1, x+2)) << 5) |
					// // Row 5
					// ((centoid > input(y+2, x-2)) << 4) |
					// ((centoid > input(y+2, x-1)) << 3) |
					// ((centoid > input(y+2, x)) << 2) |
					// ((centoid > input(y+2, x+1)) << 1) |
					// (centoid > input(y+2, x+2));
			}
	}
	
	// template <>
	// void Census::transform5x5<char>(const cv::Mat_<char>& input, cv::Mat_<unsigned int>* output);
}

#endif
