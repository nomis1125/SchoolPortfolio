import numpy as np


def linear_svm(W, b, x, y):
    """Linear SVM loss function.

    Parameters
    ----------
    W : ndarray
        The weight parameters of the linear classifier. D x C, where C is the
        number of classes, and D is the dimension of input data.

    b : ndarray
        The bias parameters of the linear classifier. C, where C is the number
        of classes.

    x : ndarray
        Input data that we want to predict the labels of. NxD, where D is the
        dimension of the input data.

    y : ndarray
        Ground truth labels associated with each sample. N numbers where each
        number corresponds to a class.

    Returns
    -------
    loss : float
        The average loss coming from this model. In the lecture slides,
        represented as \frac{1}{N}\sum_i L_i.
    """

    # TODO: implement the function

    # All class scores
    j = np.dot(x, W) + b
    
    # Score for correct class to be subtracted from j
    yi = np.reshape(j[np.arange(len(y)), y], (-1,1))

    # Li = sum of j!=yi max 0, sj-sy + 1
    Li = np.sum(np.maximum(0, j - yi + 1), axis = 1)-1

    # Mean -> 1/N sum of i to N Li
    loss = np.mean(Li, axis = 0)

    return loss


def logistic_regression(W, b, x, y):
    """Logistic regression loss function.

    Parameters
    ----------
    W : ndarray
        The weight parameters of the linear classifier. D x C, where C is the
        number of classes, and D is the dimension of input data.

    b : ndarray
        The bias parameters of the linear classifier. C, where C is the number
        of classes.

    x : ndarray
        Input data that we want to predict the labels of. NxD, where D is the
        dimension of the input data.

    y : ndarray
        Ground truth labels associated with each sample. N numbers where each
        number corresponds to a class.

    Returns
    -------
    loss : float
        The average loss coming from this model. In the lecture slides,
        represented as \frac{1}{N}\sum_i L_i.

    """

    # TODO: implement the function

    # All class scores
    j = np.dot(x, W) + b

    # ci = max j
    ci = np.max(j)

    #        e^si-ci    /  sum j e^j-ci
    twoEs = np.exp(j-ci) / np.sum(np.exp(j-ci), axis = 1, keepdims = True)

    # Li = -log()
    Li = -np.log(twoEs[range(len(y)), y])

    # Mean -> 1/N sum of i to N Li
    loss = np.mean(Li)

    return loss
