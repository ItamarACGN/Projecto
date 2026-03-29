import symnmf 
import kmeans
from kmeans import Vector
import numpy as np
from sklearn.metrics import silhouette_score
from sys import argv

def print_error_and_exit():
    print("An Error Has Occurred")
    exit(1)

def read_input() -> tuple[np.ndarray, int, int, int]:
    """
    Proccessing the cmd arrgs and validating them. 
    If the input is not valid, call print_error_and_exit().
    @ret data - a numpy array of the points for N - the number of points,
    k_val - the number of clusters, d - dimensions,
    goal -  the function to run according to the input goal.
    """
    if len(argv) != 3:
        print_error_and_exit()

    raw_k = argv[1]
    raw_file = argv[2]
    #file reading using helper function
    data, N, d = symnmf.file_to_matrix(raw_file)
    if data is None or N is None:
        print_error_and_exit()

    
    #Validate K - needs to be positive int less than N
    if not raw_k.isdigit() or int(raw_k) <= 0 or int(raw_k) >= N:
        print_error_and_exit()

    k_val = int(raw_k)
    return data, N, k_val, d


def get_kmeans_clusters(X : np.ndarray, k: int) -> np.ndarray:
    """
    Gets the cluster assignments for each data point using K-means.
    @param X: The input data.
    @param k: The number of clusters.
    @return: A numpy array of cluster assignments for each data point.
    """
    X_as_vectors = kmeans.data_to_vectors(X)
    kmeans_clusters = kmeans.kmeans_on_vectors(X_as_vectors, k) 
    kmeans_clusters = [[list(point.vector) for point in centroid] for centroid in kmeans_clusters] 
    kmeans_labels = np.zeros(len(X), dtype=int)
    for cluster_id, cluster_vectors in enumerate(kmeans_clusters):
        cluster_vectors = np.array(cluster_vectors)
        # Broadcasting: Compares every row in X with every row in the cluster
        # X[:, None, :] reshapes to (N, 1, D) to compare against (M, D)
        matches = (X[:, None, :] == cluster_vectors).all(axis=-1)
        # Get the row indices in X that matched any vector in this cluster
        row_indices = np.any(matches, axis=1)
        # Assign the cluster ID
        kmeans_labels[row_indices] = cluster_id
    return kmeans_labels

def score_algos(X : np.ndarray, N: int, k: int, d: int) -> tuple[float, float]:
    """
    Calculates the silhouette scores for both K-means and SymNMF algorithms.
    @param X: The input data.
    @param N: The number of data points.
    @param k: The number of clusters.
    @param d: The number of dimensions.
    @return: The silhouette scores for K-means and SymNMF.
    """
    kmeans_clusters = get_kmeans_clusters(X, k)
    symnmf_clusters = get_symnmf_clusters(X, N, k, d)
    symnmf_score = silhouette_score(X, symnmf_clusters)
    kmeans_score = silhouette_score(X, kmeans_clusters)
    return kmeans_score, symnmf_score


def get_symnmf_clusters(X : np.ndarray, N: int, k: int, d: int) -> list[int]:
    """
    uses the symnmf module to get H, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param X: The input data.
    @param N: The number of data points.
    @param k: The number of clusters.
    @param d: The number of dimensions.
    @return: A list of cluster assignments for each data point.
    """
    X = X.tolist()
    H = symnmf.execute_goal(X,N,k,d,"symnmf")
    return np.argmax(H, axis=1).tolist()

if __name__ == "__main__":
    """"
    Read the input, run score_algos to calculate the silhouette scores, and print the results.
    """
    X, N, k, d = read_input()
    kmeans_score ,  symnmf_score = score_algos(X, N, k, d)
    print(f"nmf: {symnmf_score}")
    print(f"kmeans: {kmeans_score}")