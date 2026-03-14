import symnmf 
import kmeans
from kmeans import Vector
import numpy as np
from sklearn.metrics import silhouette_samples
from sys import argv

def print_error_and_exit():
    print("An Error Has Occurred")
    exit(1)

def read_input():
    """
    Proccessing the cmd arrgs and validating them. 
    If the input is not valid we print "An Error Has Occurred" and return None.
    @ret data - a numpy array of the points for, N - the number of points,
    k_val - the number of clusters, d - dimensions,
    goal -  the function to run according to the input goal.
    """
    # 1. Validate input length
    if len(argv) != 3:
        print_error_and_exit()

    # 2. Assign variables based on input length
    raw_k = argv[1]
    raw_file = argv[2]
    # 4. file reading using helper function
    data, N, d = symnmf.file_to_matrix(raw_file)
    if data is None or N is None:
        print_error_and_exit()

    
    # 5. Validate K - needs to be positive int less than N
    if not raw_k.isdigit() or int(raw_k) <= 0 or int(raw_k) >= N:
        print_error_and_exit()

    k_val = int(raw_k)
    return data, N, k_val, d


def get_kmeans_clusters(X , k):
    data = kmeans.data_to_vectors(X)
    kmeans_clusters = kmeans_on_vectors(X, k) #gets the clusters as a list of lists of vectors
    kmeans_clusters = [[vector.vector for vector in centroid] for centroid in kmeans_centroids] # turns the vectors inro tuples
    kmeans_labels = []
    for cluster_id in range(len(kmeans_clusters)):
        for vector in kmeans_clusters[cluster_id]:
            vector_id = X.index(vector)
        kmeans_labels[vector_id] = cluster_id
    return kmeans_labels

def score_algos(X, k):
    kmeans_labels = get_kmeans_clusters(X)
    symnmf_labels = get_symnmf_clusters()
    symnmf_score = silhouette_samples(X, symnmf_clusters)
    kmeans_score = silhouette_samples(X, kmeans_clusters)
    return kmeans_score, symnmf_score


def run_algos():
 
    data = data.tolist()
    H = symnmf.execute_goal(data,N,k_val,d,"symnmf")
    symnmf_clusters = get_clusters(H) 

    symnmf_score = silhouette_samples(data, symnmf_clusters)
    print(symnmf_score)


def get_symnmf_clusters(H) -> list[int]:
    """
    uses the symnmf module to get H, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param H: The H matrix as a numpy array.
    @return: A list of cluster assignments for each data point.
    """
    H = 
    return np.argmax(H, axis=1).tolist()

if __name__ == "__main__":
    data, N, k_val, d = read_input()
    data = data.tolist()
    kmenas_score ,  symnmf_score = score_algos()
    print(f"nmf: {symnmf_score}")
    print(f"kmeans: {kmeans_score}")