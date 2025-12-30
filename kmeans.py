from sys import stdin, argv
import math

EPSILON = 0.001
DEFAULT_ITER = 400
MAX_ITER = 800

"""basic vector class"""
class Vector:


    def __init__(self, data):
        self.vector = tuple(data)
        self.dim = len(data)


    def __sub__(self, other):
        return Vector([self.vector[i] - other.vector[i] for i in range(self.dim)])


    def __add__(self, other):
        return Vector([self.vector[i] + other.vector[i] for i in range(self.dim)])


    """scaler multiplication"""
    def __mul__(self, other):
        return sum([self.vector[i] * other.vector[i] for i in range(self.dim)])


    def __str__(self):
        nums_strings = ["%.4f" % x for x in self.vector]
        ret = "("
        for x in nums_strings:
            ret += x + ", "
        return ret[:-2] + ")"


    def __repr__(self):
        return self.__str__()
    

    def __truediv__(self, scaler):
        return Vector([x / scaler for x in self.vector])

    @staticmethod
    def norm(v):
        return math.sqrt(v * v)


    @staticmethod
    def d(v1, v2):
        return Vector.norm(v1 - v2)

    """claculates the center of mass of a cluster - a list of vectors"""
    @staticmethod
    def centroid(cluster):
        sum = Vector([0] * cluster[0].dim)
        for v in cluster:
            sum += v
        return sum / len(cluster)


def kmeans_on_vectors(vectors, k , iter = DEFAULT_ITER):
    #initialazing clusters:
    clusters = [[vectors[i]] for i in range(k)] 
    next_clusters = [[] for i in range(k)]

    for i in range(iter):
        centroids = [Vector.centroid(cluster) for cluster in clusters]
        for v in vectors:
            #calculating distances from centroids of every cluster
            distances = [Vector.d(v, cen) for cen in centroids]
            #finding the index of the closest cluster
            id_min = distances.index(min(distances))
            #adding the vector to the closest cluster
            next_clusters[id_min].append(v)
        

        #if one of the clusters is empty we transfer a data point to it:
        for cluster in next_clusters:
            if len(cluster) == 0:
                #finding a data point in a cluster that has more than 1 point:
                for j in range(len(clusters)):
                    if len(clusters[j]) > 1:
                        v = clusters[j][0]
                        clusters[j] = clusters[j][1:]
                        cluster.append(v)
                        break
        
        #checking the deltas of the centroids
        change = [Vector.d(Vector.centroid(clusters[i]), Vector.centroid(next_clusters[i])) for i in range(k)]

        if (max(change) < EPSILON):
            return [Vector.centroid(cluster) for cluster in next_clusters]
    
        clusters = next_clusters.copy()

        #emptying the next clusters for the next iteration
        next_clusters = [[] for i in range(k)]
        
    return [Vector.centroid(cluster) for cluster in clusters]


def stdin_to_vectors():
    vectors = []
    for line in stdin:
        if line != "\n":
            nums = line.split(",")
            nums = [float(x) for x in nums]
            vectors.append(Vector(nums))
    return vectors


def print_output(vectors):
    for v in vectors:
        print(str(v)[1 : -1])

def main():
    if 3 < len(argv) or 2 > len(argv):
        print("An Error Has Occurred")
        return 
    #processing the data points to vectors list
    vectors = stdin_to_vectors()
    N = len(vectors)
    k = argv[1]
    #checking k validity
    if k.isdigit() and int(k) < N and 1 < int(k):
        k = int(k)
    else:
        print("Incorrect number of clusters!")
        return
    #if iter is given we check its validity as well
    if len(argv) == 3:
        iter = argv[2]
        if iter.isdigit() and int(iter) < MAX_ITER and 1 < int(iter):
            iter = int(iter)
        else:
            print("Incorrect maximum iterations!")
            return
    #if iter is not given we assign it to the default value        
    else:
        iter = DEFAULT_ITER
    res = kmeans_on_vectors(vectors, k, iter)
    print_output(res)


if __name__ == "__main__":

    main()
