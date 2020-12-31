# solution.py ---
#
# Filename: solution.py
# Description:
# Author: Kwang Moo Yi
# Maintainer:
# Created: Sun Jan 14 20:32:47 2018 (-0800)
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


import matplotlib.pyplot as plt
import numpy as np

from config import get_config, print_usage, print_config
from utils.cifar10 import load_data
from utils.features import extract_h_histogram, extract_hog
from utils.preprocess import normalize
from utils.regularizor import l2_grad, l2_loss
from utils import loss_func, grad_func


def compute_feature(input, config):
    if config.feature_type == "hog":
        # HOG features
        output = extract_hog(input)
    elif config.feature_type == "h_histogram":
        # Hue Histogram features
        output = extract_h_histogram(input)
    elif config.feature_type == "rgb":
        # raw RGB features
        output = input.astype(float).reshape(len(input), -1)
    return output


def compute_loss(W, b, x, y, config):
    """Computes the losses for each module."""

    # Lazy import of propper model
    if config.model_type == "linear_svm":
        model_loss = loss_func.linear_svm
    elif config.model_type == "logistic_regression":
        model_loss = loss_func.logistic_regression
    else:
        raise ValueError("Wrong model type {}".format(
            config.model_type))

    loss, loss_c, pred = model_loss(W, b, x, y)
    loss += config.reg_lambda * l2_loss(W)

    return loss, loss_c, pred


def compute_grad(W, x, y, loss_c, config):
    """Computes the gradient for each module."""

    # Lazy import of propper model
    if config.model_type == "linear_svm":
        model_grad = grad_func.linear_svm_grad
    elif config.model_type == "logistic_regression":
        model_grad = grad_func.logistic_regression_grad
    else:
        raise ValueError("Wrong model type {}".format(
            config.model_type))

    dW, db = model_grad(loss_c, x, y)
    dW += config.reg_lambda * l2_grad(W)

    return dW, db


def predict(W, b, x, config):
    """Predict function.

    Returns the correct class number, given the weights and biases of the
    linear classifier.

    Parameters
    ----------
    W : ndarray
        The weight parameters of the linear classifier. D x C, where C is the
        number of classes, and D is the dimenstion of input data.

    b : ndarray
        The bias parameters of the linear classifier. C, where C is the number
        of classes.

    x : ndarray
        Input data that we want to predic the labels of. NxD, where D is the
        dimension of the input data.

    config : namespace
        Arguments and configurations parsed by `argparse`

    Returns
    -------
    pred : ndarray
        Predictions from the model. N numbers where each number corresponds to
        a class.

    """

    # Scores for all class (N, 10)
    s_all = np.matmul(x, W) + b
    # Predections to use later
    pred = np.argmax(s_all, axis=1)

    return pred


def train(x_tr, y_tr, x_va, y_va, config):
    """Training function.

    Parameters
    ----------
    x_tr : ndarray
        Training data.

    y_tr : ndarray
        Training labels.

    x_va : ndarray
        Validation data.

    y_va : ndarray
        Validation labels.

    config : namespace
        Arguments and configurations parsed by `argparse`

    Returns
    -------
    train_res : dictionary
        Training results stored in a dictionary file. It should contain W and b
        when best validation accuracy was achieved, as well as the average
        losses per epoch during training, and the average accuracy of each
        epoch to analyze how training went.
    """

    # ----------------------------------------
    # Preprocess data

    # Report data statistic
    print("Training data before: mean {}, std {}, min {}, max {}".format(
        x_tr.mean(), x_tr.std(), x_tr.min(), x_tr.max()
    ))

    # Normalize data using the normalize function. Note that we are remembering
    # the mean and the range of training data and applying that to the
    # validation/test data later on.
    x_tr_n, x_tr_mean, x_tr_range = normalize(x_tr)
    x_va_n, _, _ = normalize(x_va, x_tr_mean, x_tr_range)
    # Always a good idea to print some debug messages
    print("Training data after: mean {}, std {}, min {}, max {}".format(
        x_tr_n.mean(), x_tr_n.std(), x_tr_n.min(), x_tr_n.max()
    ))

    # ----------------------------------------
    # Initialize parameters of the classifier
    print("Initializing...")
    num_class = 10
    # TODO (4 points): Initialize W to very small random values. e.g. random values
    # between -0.001 and 0.001. To get results identical to the one in the
    # assignment package, you should initialize with uniform random numbers
    # between this range.
    W = ((np.random.rand(len(x_tr[0]), num_class)*2)-1)/1000#Dimension of input x Class
    # TODO (1 point): Initialize b to zeros
    b = np.zeros(num_class)#of classes (num_class)
    # Test on validation data
    print("Testing...")
    pred = predict(W, b, x_va_n, config)
    acc = np.mean(pred == y_va)
    print("Initial Validation Accuracy: {}%".format(acc * 100))

    batch_size = config.batch_size
    num_epoch = config.num_epoch
    num_batch = len(x_tr_n) // batch_size
    loss_epoch = []
    tr_acc_epoch = []
    va_acc_epoch = []
    W_best = None
    b_best = None
    best_acc = 0
    # For each epoch
    for idx_epoch in range(num_epoch):
        # TODO (5 points): Create a random order to go through the data. Note
        # that we pre-create the random order and then proceed through the
        # data. Recently, people often simply grab data purely randomly from
        # the entire dataset. However, we'll stick to the more traditional way.
        order = np.random.randint(0, high = len(x_tr_n), size = batch_size)
        # For each training batch
        losses = np.zeros(num_batch)
        accs = np.zeros(num_batch)
        for idx_batch in range(num_batch):
            # TODO (5 points): Construct batch
            x_b = x_tr_n[order]
            y_b = y_tr[order]
            # Get loss with compute_loss
            loss_cur, temp_b, pred_b = compute_loss(W, b, x_b, y_b, config)
            # Get gradient with compute_grad
            #
            # HINT: When implementing the gradient function, it is a good idea
            # to first set your epochs to a low value e.g. 5, so that you see
            # the loss going down immediately. 100 epochs takes quite a while.
            dW, db = compute_grad(W, x_b, y_b, temp_b, config)
            # TODO (5 points): Update parameters (use `config.learning_rate`)
            W = W - (dW * config.learning_rate) # = 1e-4
            b = b - (db * config.learning_rate)
            # Record this batches result
            losses[idx_batch] = loss_cur
            accs[idx_batch] = np.mean(pred_b == y_b)

        # Report average results within this epoch
        print("Epoch {} -- Train Loss: {}".format(
            idx_epoch, np.mean(losses)))
        print("Epoch {} -- Train Accuracy: {:.2f}%".format(
            idx_epoch, np.mean(accs) * 100))

        # TODO (5 points): Test on validation data and report results
        pred = predict(W, b, x_va_n, config)
        acc = np.mean(pred == y_va)
        print("Epoch {} -- Validation Accuracy: {:.2f}%".format(
            idx_epoch, acc * 100))

        # TODO (2 points): If best validation accuracy, update W_best, b_best,
        # and best accuracy. We will only return the best W and b later.
        if acc > best_acc:
            W_best = W
            b_best = b
            best_acc = acc
        # Record per epoch statistics
        loss_epoch += [np.mean(losses)]
        tr_acc_epoch += [np.mean(accs)]
        va_acc_epoch += [acc]

    # TODO (3 points): Pack results for the best validation as a dictionary. Remeber to pack
    # pre-processing related things here as well. We currently pack only three
    # things, but you probably want to pack more things.
    train_res = {}
    train_res["tr_acc_epoch"] = tr_acc_epoch
    train_res["va_acc_epoch"] = va_acc_epoch
    train_res["loss_epoch"] = loss_epoch
    train_res["W_best"] = W_best
    train_res["b_best"] = b_best
    train_res["best_acc"] = best_acc
    train_res["x_tr_mean"] = x_tr_mean
    train_res["x_tr_range"] = x_tr_range

    return train_res


def main(config):
    print_config(config)
    """The main function."""

    # ----------------------------------------
    # Load cifar10 train data
    print("Reading training data...")
    data_trva, y_trva = load_data(config.data_dir, "train")

    # ----------------------------------------
    # Load cifar10 test data
    print("Reading test data...")
    data_te, y_te = load_data(config.data_dir, "test")

    # ----------------------------------------
    # Extract features
    print("Extracting Features...")
    x_trva = compute_feature(data_trva, config)
    x_te = compute_feature(data_te, config)

    # ----------------------------------------
    # Create folds
    num_fold = config.cv_num_folds

    # TODO (5 points): Randomly shuffle data and labels. IMPORANT: make sure
    # the data and label is shuffled with the same random indices so that they
    # don't get mixed up!
    print("Shuffling")

    p1 = np.random.permutation(len(x_trva))

    x_trva = x_trva[p1]
    y_trva = y_trva[p1]

    # Cross validation setup. Note here that if we set cross validation to
    # False, which is the default, we'll just return results for the first
    # fold.
    if config.cross_validate:
        va_fold_to_test = np.arange(num_fold)
    else:
        va_fold_to_test = np.arange(1)

    # ----------------------------------------
    # Cross validation loop
    #
    # HINT: To first be able to implement without the cross validation in
    # place, you can first use the full training data as the traning set, and
    # the test data as the validation set, just to get the program running. To
    # do that, simply set `x_tr=x_trva` and `x_va=x_te`. Same for the y
    # labels. Once you have the framework running, you can then implement the
    # cross validation loop.

    
    numi = (int)(len(x_trva) / num_fold)
    seperateData = np.empty((num_fold, numi, len(x_trva[0])))
    seperateLab = np.empty((num_fold, numi), dtype = int)

    for i in range(0, num_fold):
        pos = np.arange(0, numi) + i * numi
        seperateData[i] = x_trva[pos]
        seperateLab[i] = y_trva[pos]

    train_res = []
    for idx_va_fold in va_fold_to_test:
        # TODO (10 points): Create train and validation data using folds. Note
        # that not all of this process needs to be here. You can also have some
        # parts of this implemented outside of the for loop.
        if config.cross_validate:
            pos = [i for i in range(0,num_fold) if i != idx_va_fold]
            x_tr = np.stack(seperateData[pos])[0]
            y_tr = np.stack(seperateLab[pos])[0]
            x_va = x_te[idx_va_fold]
            y_va = y_te[idx_va_fold]
        else:
            x_tr = x_trva
            y_tr = y_trva
            x_va = x_te
            y_va = y_te
        # ----------------------------------------
        # Train
        print("Training for fold {}...".format(idx_va_fold))
        # Run training
        cur_train_res = train(x_tr, y_tr, x_va, y_va, config)

        # Save results
        train_res += [cur_train_res]

    # Average results from all folds and display. Note that this is not
    # necesarilly the best way to display things
    tr_acc_epoch = np.mean([
        _res["tr_acc_epoch"] for _res in train_res
    ], axis=0)
    va_acc_epoch = np.mean([
        _res["va_acc_epoch"] for _res in train_res
    ], axis=0)
    loss_epoch = np.mean([
        _res["loss_epoch"] for _res in train_res
    ], axis=0)

    # Draw train and validation accuracies
    plt.figure()
    plt.plot(np.arange(config.num_epoch), tr_acc_epoch)
    plt.plot(np.arange(config.num_epoch), va_acc_epoch)
    plt.show(False)

    # Draw loss evolution
    plt.figure()
    plt.plot(np.arange(config.num_epoch), loss_epoch)
    plt.show()

    # Different final execution based on run type
    if config.cross_validate:
        # TODO (5 points): If we are cross validating, simply report the
        # average of all maximum validation accuracy. We are tuning hyper
        # parameters, and to get a model that we want to test later, we need to
        # have retrain with the best hyperparameter setup.
        for i in range(0, num_fold):
            ttl = ttl + train_res[i]["best_acc"]
        val_acc = ttl/num_fold
        print("Average best validation accuracy: {}%".format(
            val_acc * 100))

    else:
        assert len(train_res) == 1
        # TODO (5 points): Get its W, b, x_tr_mean, x_tr_range
        # TODO (5 points): Test the model
        W = train_res[0]["W_best"]
        b = train_res[0]["b_best"]
        x_tr_mean = train_res[0]["x_tr_mean"]
        x_tr_range = train_res[0]["x_tr_range"]

        pred = predict(W, b, x_te, config)
        acc = np.mean(pred == y_te)
        print("Test Accuracy: {}%".format(acc * 100))


if __name__ == "__main__":

    # ----------------------------------------
    # Parse configuration
    config, unparsed = get_config()
    # If we have unparsed arguments, print usage and exit
    if len(unparsed) > 0:
        print_usage()
        exit(1)

    main(config)


#
# solution.py ends here