import logging
import pandas
import numpy
import sklearn
import math
import heapq
import matplotlib.pyplot as plot
import time

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

def itemDistance(item1, item2, table):
    """calculate similarity (cosine distance of two items)
    :param user1: id of the first user / a list of item scores from the first user
    :param user2: id of the second user
    :param table: original table
    :return: distance of two user in floating point
    """
    itemUserTable = table.T
    return numpy.dot(itemUserTable[item1], itemUserTable[item2]) / (numpy.linalg.norm(itemUserTable[item1]) * numpy.linalg.norm(itemUserTable[item2]))


def itemDistanceTable(table):
    """get item-item distance table from original table
    :param table: the original table
    :return: item-item similarity matrix
    """
    result = [[0 for col in range(table.shape[1])] for row in range(table.shape[1])]
    for item1 in range(table.shape[1]):
        for item2 in range(table.shape[1]):
            if item1 == item2:
                result[item1][item2] = 0
            else:
                distance = itemDistance(item1, item2, table)
                result[item1][item2] = distance
                result[item2][item1] = distance
    for index in range(len(result)):
        maxnum = numpy.max(result[index])
        minnum = numpy.min(result[index])
        for column in range(len(result)):
            if result[index][column] > 0 and maxnum:
                result[index][column] /= maxnum
            elif minnum:
                result[index][column] /= minnum
    return numpy.array(result)

def recommend(items, itemDisTable):
    """recommend items for user
    :param items: the items this user already scored
    :param table: original data table (training set)
    :param k: k nearest neighbor
    :return: estimated items and score, in a dictionary of item: estimate-score
    """
    result = {}
    for itemIndex in range(len(itemDisTable)):
        if items[itemIndex] == 0:
            result[itemIndex] = 0
            # calculate via scored items
            base = 0
            for scoredItemIndex in range(len(items)):
                if items[scoredItemIndex] != 0:
                    result[itemIndex] += itemDisTable[itemIndex][scoredItemIndex] *  items[scoredItemIndex]
                    base += itemDisTable[itemIndex][scoredItemIndex]
            if base:
                result[itemIndex] /= base
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
    itemDisTable = itemDistanceTable(data)

    testData = readData("data/jester-data-2.csv")
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
            result = recommend(testRemoved, itemDisTable)

            # calculate NMAE
            err = 0
            count = 0
            rMax = -10
            rMin = 10
            if len(test):
                for index in result:
                    if test[index] != 0:
                        # # update rmax and rmin
                        # if(result[index] > rMax):
                        #     rMax = result[index]
                        # if(result[index] < rMin):
                        #     rMin = result[index]
                        # error
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
    itemDisTable = itemDistanceTable(data)

    testData = readData("data/jester-data-2.csv")
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
        result = recommend(testRemoved, itemDisTable)

        # calculate NMAE
        err = 0
        count = 0
        rMax = -10
        rMin = 10
        if len(test):
            for index in result:
                if test[index] != 0:
                    # error
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
