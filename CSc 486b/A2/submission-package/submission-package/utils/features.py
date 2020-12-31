# features.py ---
#
# Filename: features.py
# Description:
# Author: Kwang Moo Yi
# Maintainer:
# Created: Sun Jan 14 21:06:57 2018 (-0800)
# Version:
# Package-Requires: ()
# URL:
# Doc URL:
# Keywords:
# Compatibility:
#
#

# Commentary:
#
#
#
#

# Change Log:
#
#
#
# Copyright (C), Visual Computing Group @ University of Victoria.

# Code:

import numpy as np
from skimage.color import rgb2hsv
from skimage.feature import hog


def extract_h_histogram(data):
    """Extract Hue Histograms from data.

    Parameters
    ----------
    data : ndarray
        NHWC formatted input images.

    Returns
    -------
    h_hist : ndarray (float)
        Hue histgram per image, extracted and reshaped to be NxD, where D is
        the number of bins of each histogram.

    """

    # Using Hist
    print("Extracting Hist...")

    # TODO: Implement the method

    #convert the data to hsv
    data2 = [None] * len(data)
    #print(data.shape)
    i = 0
    while i < len(data2):
        data2[i] = rgb2hsv(data[i])
        i = i + 1
    hsv = np.asarray(data2)
    #print(hsv.shape)

    #gets us the hue
    hue = hsv[:,:,:,0]
    #print(hue.shape)

    #convert to histogram and to have 16 bins
    h_hist = [None] * len(hue)#hue
    k = 0
    while k < len(h_hist):
        h_hist[k] = np.histogram(hue[k],bins = 16, range = (0,1))[0]
        k = k + 1
    h_hist = np.asarray(h_hist)

    #print(h_hist.shape)

    # Assertion to help you check if implementation is correct
    assert h_hist.shape == (data.shape[0], 16)

    return h_hist


def extract_hog(data):
    """Extract HOG from data.

    Parameters
    ----------
    data : ndarray
        NHWC formatted input images.

    Returns
    -------
    hog_feat : ndarray (float)
        HOG features per image, extracted and reshaped to be NxD, where D is
        the dimension of HOG features.

    """

    # Using HOG
    print("Extracting HOG...")

    # TODO: Implement the method

    #makes the image grey
    data2 = np.average(data, axis=-1)
    
    #creates the HOG Image for all images
    can = [None]*len(data2)
    i = 0
    while(i<len(data2)):
        can[i] = hog(data2[i])
        i = i + 1
    can = np.asarray(can)

    hog_feat = can

    # Assertion to help you check if implementation is correct
    assert hog_feat.shape == (data.shape[0], 324)

    return hog_feat


#
# features.py ends here
