import matplotlib.pyplot as plot
import numpy
import heapq
from PIL import Image
from PIL import ImageFilter

# initialize datas
dataDir="data"
faceDirs=[]
for i in range(1, 41):
    faceDirs.append("s" + str(i))

faces=[]
for i in range(1, 11):
    faces.append(str(i) + ".pgm")

def imageToVector(path):
    """
    conver image to vector
    :param path: path to the image
    :return: numpy vector
    """
    img = numpy.array(Image.open(path)) #.convert('1'))
    #img = numpy.array(Image.open(path).filter(ImageFilter.MedianFilter()).filter(ImageFilter.CONTOUR))
    return img.flatten()

def initializeTrainSet(faces):
    """
    initialize training data set
    :param faces: a list of face ids to use as train set
    :return: the initalized data set, a list of (data, label) tuple
    """
    trainDataSet = []
    for faceDir in faceDirs:
        for face in faces:
            imgData = imageToVector(dataDir + "/" + faceDir + "/" + str(face) + ".pgm")
            trainDataSet.append((imgData, faceDir))
    return trainDataSet

def distance(vec1, vec2):
    """
    calculate eular distance of two vectors
    :param vec1: vector 1
    :param vec2: vector 2
    :return: distance
    """
    #return numpy.sum(vec1^vec2) #strange...
    return numpy.sum(numpy.square(vec1-vec2)) # yeah...


def recognize(face, trainDataSet, k):
    """
    calculate which face in training dataset is identical to this face, using
    k-NN algorithm

    :param face: the testing face in numpy array
    :param trainDataSet: trained data
    :param k: k-NN parameter
    :return: the most identical face in dataset
    """
    heap = []
    result = {}
    # calculate k nearest neighbors
    for trainData in trainDataSet:
        heapq.heappush(heap, (distance(face, trainData[0]), trainData[1]) )

    # find the label appeared maximum times
    for i in range(k):
        top = heapq.heappop(heap)[1]
        if top in result:
            result[top] = result[top] + 1
        else:
            result[top] = 1
    print(result)

    maximum = (None, 0)
    for label in result:
        if result[label] > maximum[1]:
            maximum = (label, result[label])

    return maximum[0]

def main():
    correctCount = 0
    totalCount = 0

    # different k
    kList = []
    correctRateList = []
    for k in range(1, 21):
        # 10-fold cross validation
        for testIndex in range(1, 11):
            # initialize train dataset
            trainImages = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
            trainImages.remove(testIndex)
            trainDataSet = initializeTrainSet(trainImages)

            # recognize and validate
            for faceDir in faceDirs:
                # recognize face
                faceData = imageToVector(dataDir + "/" + faceDir + "/" + str(testIndex) + ".pgm")
                result = recognize(faceData, trainDataSet, k)
                print(faceDir, ": ", result)

                # calculate correct rate
                if faceDir == result:
                    correctCount = correctCount + 1
                totalCount = totalCount + 1
        kList.append(k)
        correctRateList.append(correctCount / totalCount)
        # print correct rate
        print("Correct rate:", correctCount / totalCount)
    print(kList)
    print(correctRateList)

    plot.bar(kList, correctRateList, align='center', alpha=0.5)
    plt.xticks(kList, kList)
    plt.ylabel("Correct Rate")
    plot.show()

if __name__ == "__main__":
    main()

