/*
 * Author: Konstantin Schauwecker
 * Year:   2012
 */

#include "census-inl.h"

namespace sparsestereo {
	using namespace std;
	using namespace cv;

	template <>
	void Census::transform5x5<char>(const Mat_<char>& input, Mat_<unsigned int>* output,int edgeNo) {
			transform5x5SSE(input, output);
		//censusTransform(input,output) ; 
	}
	
// 	void censusTransform(const Mat_<char>& imgIn, Mat_<unsigned int>* imgTemp)
// 	{
// 		unsigned int census = 0;
// 		unsigned int bit = 0;
// 		 Size imgSize = imgIn.size();
// 		 imgTemp = Mat::zeros(imgSize, CV_8U);

// 		 int m = 3;
// 		  int n = 3;//window size
// 		  int i,j,x,y;
// 		  int shiftCount = 0;


// 		  for (x = m/2; x < imgSize.height - m/2; x++)
// 		  {
// 		    for(y = n/2; y < imgSize.width - n/2; y++)
// 		    {

// 		    	census = 0;
// 		    	     shiftCount = 0;
// 		    	     for (i = x - m/2; i <= x + m/2; i++)
// 		    	     {
// 		    	       for (j = y - n/2; j <= y + n/2; j++)
// 		    	       {

// 		    	         if( shiftCount != m*n/2 )//skip the center pixel
// 		    	         {
// 		    	         census <<= 1;
// 		    	         if( imgIn.at<unsigned int>(i,j) < imgIn.at<unsigned int>(x,y) )//compare pixel values in the neighborhood
// 		    	         bit = 1;
// 		    	         else
// 		    	         bit = 0;
// 		    	         census = census + bit;
// 		    	         //cout<<census<<" ";*/

// 		    	         }
// 		    	         shiftCount ++;
// 		    	       }
// 		    	     }
// 		    	     imgTemp.ptr<unsigned int>(x)[y] = census;
// 		    }

// }
// 	}



void Census::transform5x5SSE(const Mat_<char>& input, Mat_<unsigned int>* output) {
	int maxX=input.cols-2, maxY=input.rows-2;

		for(int y=2; y<maxY; y++)
			for(int x=2; x<maxX; x++) {
				char centoid = input(y,x);

				(*output)(y,x) =
					// Row 1
					//((centoid > input(y-2, x-2)) << 24) |
					// ((centoid > input(y-2, x-1)) << 23) |
					((centoid > input(y-2, x)) << 22) |
					((centoid > input(y-2, x+1)) << 21) |
					((centoid > input(y-2, x+2)) << 20) |
					// Row 2
					((centoid > input(y-1, x-2)) << 19) |
					((centoid > input(y-1, x-1)) << 18) |
					((centoid > input(y-1, x)) << 17) |
					((centoid > input(y-1, x+1)) << 16) |
					((centoid > input(y-1, x+2)) <<15) |
					//Row 3
					((centoid > input(y, x-2)) << 14) |
					((centoid > input(y, x-1)) << 13) |
					((centoid > input(y, x)) << 12) |
					((centoid > input(y, x+1)) << 11) |
					((centoid > input(y, x+2)) << 10) |
					// Row 4
					((centoid > input(y+1, x-2)) << 9) |
					((centoid > input(y+1, x-1)) << 8) |
					((centoid > input(y+1, x)) << 7) |
					((centoid > input(y+1, x+1)) << 6) |
					((centoid > input(y+1, x+2)) << 5) |
					// Row 5
					((centoid > input(y+2, x-2)) << 4) |
					((centoid > input(y+2, x-1)) << 3) |
					((centoid > input(y+2, x)) << 2) |
					((centoid > input(y+2, x+1)) << 1) |
					(centoid > input(y+2, x+2));
			}
	}
}

// 	void Census::transform5x5SSE(const Mat_<char>& input, Mat_<unsigned int>* output) {
// 		// Predeclare required constants
// 		const v16qi const01 = SIMD::scalar16(0x01);

// 		// We skip one row at the beginning and end to avoid range checking
// 		for(int y=2; y<input.rows-2; y++)
// 			for(int x=0; x<input.cols; x+=16) {
// 				// Get 16 centoids
// 				v16qi centoid = (v16qi)_mm_load_si128((__m128i*)&(input)(y,x));
				
// 				// Row 5
// 				const char* rowPtr = &input(y+2, x-2);
// 				v16qi bitConst = const01;
// 				v16qi byte1 = __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 4)) & bitConst;
// 				bitConst += bitConst; // 0x02
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 3)) & bitConst;
// 				bitConst += bitConst; // 0x04
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, (v16qi)_mm_load_si128((__m128i*)(rowPtr + 2))) & bitConst; //Alinged
// 				bitConst += bitConst; // 0x08
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 1)) & bitConst;
// 				bitConst += bitConst; // 0x10
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr)) & bitConst;
				
// 				// Row 4
// 				rowPtr -= input.step;
// 				bitConst += bitConst; // 0x20
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 4)) & bitConst;
// 				bitConst += bitConst; // 0x40
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 3)) & bitConst;
// 				bitConst += bitConst; // 0x80
// 				byte1 |= __builtin_ia32_pcmpgtb128(centoid, (v16qi)_mm_load_si128((__m128i*)(rowPtr + 2))) & bitConst; //Aligned
// 				// Byte 2 starts
// 				bitConst = const01;
// 				v16qi byte2 = __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 1)) & bitConst;
// 				bitConst += bitConst; // 0x02
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr)) & bitConst;
				
// 				// Row 3
// 				rowPtr -= input.step;
// 				bitConst += bitConst; // 0x04
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 4)) & bitConst;
// 				bitConst += bitConst; // 0x08
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 3)) & bitConst;
// 				bitConst += bitConst; // 0x10
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 1)) & bitConst;
// 				bitConst += bitConst; // 0x20
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr)) & bitConst;
				
// 				// Row 2
// 				rowPtr -= input.step;
// 				bitConst += bitConst; // 0x40
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 4)) & bitConst;
// 				bitConst += bitConst; // 0x80
// 				byte2 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 3)) & bitConst;
// 				// Byte 3 starts
// 				bitConst = const01;
// 				v16qi byte3 = __builtin_ia32_pcmpgtb128(centoid, (v16qi)_mm_load_si128((__m128i*)(rowPtr + 2))) & bitConst; //Aligned
// 				bitConst += bitConst; // 0x02
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 1)) & bitConst;
// 				bitConst += bitConst; // 0x04
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr)) & bitConst;
				
// 				// Row 1
// 				rowPtr -= input.step;
// 				bitConst += bitConst; // 0x08
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 4)) & bitConst;
// 				bitConst += bitConst; // 0x10
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 3)) & bitConst;
// 				bitConst += bitConst; // 0x20
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, (v16qi)_mm_load_si128((__m128i*)(rowPtr + 2))) & bitConst; //Aligned
// 				bitConst += bitConst; // 0x40
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr + 1)) & bitConst;
// 				bitConst += bitConst; // 0x80
// 				byte3 |= __builtin_ia32_pcmpgtb128(centoid, __builtin_ia32_loaddqu(rowPtr)) & bitConst;
												
// 				storeSSEVec(byte3^byte3 /*0*/, byte3, byte2, byte1, (char*) &(*output)(y,x));
// 			}
// 	}
	
// 	__always_inline void Census::storeSSEVec(const v16qi& byte4, const v16qi& byte3, const v16qi& byte2, const v16qi& byte1, char* dst) {
// 		// Combine bytes to shorts
// 		v8hi high1 = (v8hi) __builtin_ia32_punpcklbw128(byte3, byte4);
// 		v8hi high2 = (v8hi) __builtin_ia32_punpckhbw128(byte3, byte4);
// 		v8hi low1 = (v8hi) __builtin_ia32_punpcklbw128(byte1, byte2);
// 		v8hi low2 = (v8hi) __builtin_ia32_punpckhbw128(byte1, byte2);
		
// 		// Combine shorts to ints
// 		__builtin_ia32_storedqu(dst, (v16qi)__builtin_ia32_punpcklwd128(low1, high1));
// 		__builtin_ia32_storedqu(dst + 4*4, (v16qi)__builtin_ia32_punpckhwd128(low1, high1));
// 		__builtin_ia32_storedqu(dst + 8*4, (v16qi)__builtin_ia32_punpcklwd128(low2, high2));
// 		__builtin_ia32_storedqu(dst + 12*4, (v16qi)__builtin_ia32_punpckhwd128(low2, high2));
// 	}
// }
