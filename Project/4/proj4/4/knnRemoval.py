'''
import numpy as np
import sys
import multiprocessing

def knnRemoval(image, k, kernelSize, distance_thr):
	halfSize = int(kernelSize/2)
	angle_width_per_pixel = np.pi*2/image.shape[1]
	angle_height_per_pixel = np.pi*2/image.shape[0]
	#print(list(range(kernelSize)))
	x_grid, y_grid = np.meshgrid(list(range(kernelSize)), list(range(kernelSize)))
	#print(x_grid, y_grid)
	cnt = np.zeros((1),dtype=np.uint32)
	def knnRemovalValue(image, y, x,  k, kernelSize, distance_thr):
		center_depth = image[y,x]   
		depth_squared = center_depth**2
		width_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_width_per_pixel)*2*depth_squared) )
		height_per_pixel = np.sqrt( 2*depth_squared - (np.cos(angle_height_per_pixel)*2*depth_squared) )
		kernel = np.abs(image[y-halfSize:y+halfSize+1, x-halfSize:x+halfSize+1] - center_depth)\
				 + np.abs(width_per_pixel * (x_grid-halfSize))\
				+ np.abs(height_per_pixel * (y_grid-halfSize))
		sorted_kernel = np.sort(kernel.reshape(-1))
		sorted_kernel = sorted_kernel[np.where(sorted_kernel!=0)]
		mean = np.mean(sorted_kernel[:k])#a little bit different from cpp version here
		if(mean > distance_thr):
			cnt[0] += 1
			return 0.
		return image[y,x]	
	filtered_image = np.zeros(image.shape, dtype=image.dtype)

	pool = multiprocessing.Pool(4)
	def parallel(i):
		for j in range(image.shape[1]):
			if(i >= halfSize and j >= halfSize and i < image.shape[0] - halfSize and j < image.shape[1] - halfSize):
				if(image[i,j] != 0):
					filtered_image[i,j] = knnRemovalValue(image, i, j, k, kernelSize, distance_thr)
		sys.stdout.write("\rrow %d/%d of image"%(i, image.shape[0]))
		sys.stdout.flush()

	for i in range(image.shape[0]):
		print(i)
		pool.apply_async(parallel, args=(i,))

	pool.close()
	pool.join()

	sys.stdout.write("\n")
	
	return filtered_image, cnt[0]
'''
#coding: utf-8
import multiprocessing
import time

def func(msg):
    print "msg:", msg
    time.sleep(3)
    print "end"

if __name__ == "__main__":
    pool = multiprocessing.Pool(processes = 3)
    for i in xrange(4):
        msg = "hello %d" %(i)
        pool.apply_async(func, (msg, ))

    print "Mark~ Mark~ Mark~~~~~~~~~~~~~~~~~~~~~~"
    pool.close()
    pool.join()
    print "Sub-process(es) done."