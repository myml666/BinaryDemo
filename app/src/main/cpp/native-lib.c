#include <jni.h>
#include <android/bitmap.h>
JNIEXPORT void JNICALL Java_com_hehuidai_binarydemo_MainActivity_getBinaryBitmap
(JNIEnv* env, jobject jobj, jobject jBitmap) {
	int result;
	// 获取源Bitmap相关信息：宽、高等
	AndroidBitmapInfo sourceInfo;
	result = AndroidBitmap_getInfo(env, jBitmap, &sourceInfo);
	if (result < 0) {
		return;
	}
	// 获取源Bitmap像素数据 这里用的是32位的int类型 argb每个8位
	uint32_t* sourceData;
	//锁定像素的地址（不锁定的话地址可能会发生改变）
	result = AndroidBitmap_lockPixels(env, jBitmap, (void**)& sourceData);
	if (result < 0) {
		return;
	}
	// 遍历各个像素点
	int color;
	int red, green, blue , alpha;
	int width = sourceInfo.width;
	int height = sourceInfo.height;
	int w, h;
	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			color = sourceData[h * width + w];
			alpha = color & 0xff000000;
			red = (color & 0x00ff0000) >> 16;
			green = (color & 0x0000ff00) >> 8;
			blue = color & 0x000000ff;
			// 通过加权平均算法,计算出最佳像素值
			color = red * 0.3 + green * 0.59 + blue * 0.11;
			if (color <= 95) {
				color = 0;
			}
			else
			{
				color = 255;
			}
			sourceData[h * width + w] = alpha | (color << 16) | (color << 8) | color;
		}
	}
	AndroidBitmap_unlockPixels(env, jBitmap);
}