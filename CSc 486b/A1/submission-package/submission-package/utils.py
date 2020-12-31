# import libraries here
import matplotlib.pyplot as plt
from PIL import Image
import numpy as num
import h5py as h5
import scipy.ndimage.filters as snf

def read_image(image_path, *args, **kwargs):
    """Read an image into a numpy array given the path of the file.

    Parameters
    ----------
    image_path : string
        Path to the image file.

    Returns
    -------
    image : ndarray
        HWC formatted image.
    """
    # TODO: Implement the method
    image2 = Image.open(image_path)
    image = num.asarray(image2)

    return image


def invert_image(image, *args, **kwargs):
    """Invert the image color by subtracting the image from a white image.

    Parameters
    ----------
    image : ndarray
        Original image.

    Returns
    -------
    inv_image : numpy.ndarray
        Inverted image.
    """
    # TODO: Implement the method

    image2 = Image.fromarray(image)
    image3 = Image.eval(image2, lambda a: 255-a)
    inv_image = num.asarray(image3)

    return inv_image


def save_image_to_h5(image, h5_path, *args, **kwargs):
    """Save an image to H5 file.

    Parameters
    ----------
    image : ndarray
        The image to be saved.

    h5_path: string
        Path to the H5 file.
    """
    # TODO: Implement the method

    f = h5.File(h5_path, "w")
    grp = f.create_group("data")
    grp.create_dataset("image", data=image)


def read_image_from_h5(h5_path, *args, **kwargs):
    """Read an image into a numpy array given the path of a H5 file.

    Parameters
    ----------
    h5_path : string
        Path to the H5 file.

    Returns
    -------
    image : ndarray
        HWC formatted image.
    """
    # TODO: Implement the method

    h5_pathRd = h5.File(h5_path, "a")
    image = h5_pathRd["data"]["image"]

    return image


def gray_scale_image(image, *args, **kwargs):
    """Return the gray_scale image by taking the mean over channels.

    Parameters
    ----------
    image : ndarray
        Original image.

    Returns
    -------
    gray_scale : ndarray
        HW formmated gray scale image.
    """
    # TODO: Implement the method

    gray_scale = num.dot(image, [0.298, 0.587, 0.114])

    return gray_scale


def find_difference_of_gaussian_blur(image, k1, k2, *args, **kwargs):
    """Find the difference of two Gaussian blurs from an image.

    Parameters
    ----------
    image : ndarray
        Original image.

    k1 : scalar
        First standard deviation for Gaussian kernel.

    k2 : scalar
        Second standard deviation for Gaussian kernel.

    Returns
    -------
    res : ndarray
        Normalized difference of Gaussian blurs.
    """
    # TODO: Implement the method
    
    res1 = snf.gaussian_filter(image, k1)
    res2 = snf.gaussian_filter(image, k2)
    res = res2 - res1
    minn = num.amin(res)
    maxx = num.amax(res)
    if(minn - maxx != 0):
        res = (res-minn)/(maxx-minn)

    return res


def keep_top_percentile(image, percentile, *args, **kwargs):
    """Find the difference of two Gaussian blurs from an image.

    Parameters
    ----------
    image : ndarray
        Original image.

    percentile : scalar
        Top percentile pixels will be kept.

    Returns
    -------
    thresholded : ndarray
        Image with the high value pixles.
    """
    # TODO: Implement the method

    imageSort = num.sort(image, axis=None)
    elem = int(len(imageSort) * (percentile/100))
    big = num.zeros(elem)

    k = len(imageSort)-1
    for i in range(0, elem):
        if imageSort[k] in big:
            k -= 1
        else:
            big[i] = imageSort[k]
            k -= 1

    thresholded = num.zeros((640, 480))
    for i in range (0, 640):
        for j in range (0, 480):
            l = image[i][j]
            if l in big:
                thresholded[i][j] = image[i][j]
            else:
                thresholded[i][j] = 0

    save_image_to_h5(thresholded, "thresholded.h5")
    

    return thresholded
