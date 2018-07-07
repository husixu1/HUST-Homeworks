import logging
import pandas
import numpy
import math
import heapq
import random
import time
import matplotlib.pyplot as plot

def readData(path):
    """
    read data into 2-dimensional array
    :param path: path to the xml data file
    :return: pandas data frame
    """
    logging.info("reading data file ", path)
    dataFrame = pandas.read_csv(path)
    logging.info("data file ", path, " read")
    return numpy.delete(numpy.array(dataFrame), numpy.s_[:1], axis=1)

def userDistance(user1, user2, table):
    """calculate similarity (cosine distance of two users)
    :param user1: id of the first user / a list of item scores from the first user
    :param user2: id of the second user
    :param table: original table
    :return: distance of two user in floating point
    """
    if type(user1) == list or type(user1) == numpy.ndarray:
        return numpy.dot(user1, table[user2]) / (numpy.linalg.norm(user1) * numpy.linalg.norm(table[user2]))
    else:
        return numpy.dot(table[user1], table[user2]) / (numpy.linalg.norm(table[user1]) * numpy.linalg.norm(table[user2]))


def userDistanceTable(table):
    """get user-user distance table from original table
    :param table: the original table
    :return: user-user similarity sparse matrix,
        in the form of 2-level dictionary
    """
    #itemUserTable = table.T
    #for users in itemUserTable:
    #    for user in users:
    #        user
    pass

def recommend(table):
    """recommend items for user
    :param items: the items this user already scored
    :param table: original data table (training set)
    :param k: k nearest neighbor
    :return: estimated items and score, in a dictionary of item: estimate-score
    """
    result = {}
    for i in range(table.shape[1]):
        result[i] = random.randint(-10,10)
    return result

def preProcess(table):
    """
    do pre-processing
    :param table: original table
    :return: preprocessed table
    """
    table[table == 99] = 0
    return table

def trend():
    # read from csv file
    data = readData("data/jester-data-1.csv")
    data = preProcess(data)

    testData = readData("data/jester-data-3.csv")
    testData = preProcess(testData)

    totalError = 0
    totalCount = 0

    startTime = time.clock()

    jList = []
    avgNMAEList = []
    for j in range(10,100,10):
        for i in range(500):
            # test the nth line of data
            test = testData[i]

            # random remove something from test
            testRemoved = test.copy()
            testRemoved[:j] = [0] * j

            # get recommend list
            result = recommend(data)

            # calculate NMAE
            err = 0
            count = 0
            rMax = -10
            rMin = 10
            if len(test):
                for index in result:
                    if test[index] != 0:
                        # update rmax and rmin
                        err += math.fabs(test[index] - result[index])
                        count += 1
                if count:
                    totalError += err / count / 20
                    totalCount += 1
                    print("user ", i , ": ", err / count / 20)
                else:
                    print("user ", i , ": no recommendation")

        endTime = time.clock()
        print("average NMAE: ", totalError / totalCount)
        print("elapsed time: ", endTime - startTime)

        avgNMAEList.append(totalError / totalCount)
        jList.append(j)
    plot.scatter(jList, avgNMAEList)
    plot.show()


def main():
    # read from csv file
    data = readData("data/jester-data-1.csv")
    data = preProcess(data)

    testData = readData("data/jester-data-3.csv")
    testData = preProcess(testData)

    totalError = 0
    totalCount = 0

    startTime = time.clock()
    for i in range(500):
        # test the nth line of data
        test = testData[i]

        # random remove something from test
        testRemoved = test.copy()
        testRemoved[:50] = [0] * 50

        # get recommend list
        result = recommend(data)

        # calculate NMAE
        err = 0
        count = 0
        rMax = -10
        rMin = 10
        if len(test):
            for index in result:
                if test[index] != 0:
                    # update rmax and rmin
                    err += math.fabs(test[index] - result[index])
                    count += 1
            if count:
                totalError += err / count / 20
                totalCount += 1
                print("user ", i , ": ", err / count / 20)
            else:
                print("user ", i , ": no recommendation")

    endTime = time.clock()
    print("average NMAE: ", totalError / totalCount)
    print("elapsed time: ", endTime - startTime)


if __name__ == "__main__":
    main()
