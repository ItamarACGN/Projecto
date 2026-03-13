import symnmf 
import kmeans
from kmeans import Vector
import numpy as np
from sklearn.metrics import silhouette_samples


def get_kmeans_clusters(X):
    kmeans_clusters = kmeans.main() #gets the clusters as a list of lists of vectors
    kmeans_clusters = [[vector.vector for vector in centroid] for centroid in kmeans_centroids] # turns the vectors inro tuples
    kmeans_labels = []
    for cluster_id in range(len(kmeans_clusters)):
        for vector in kmeans_clusters[cluster_id]:
            vector_id = X.index(vector)
        kmeans_labels[vector_id] = cluster_id
    return kmeans_labels

def score_algos():
    X = get_points()
    kmeans_labels = get_kmeans_clusters(X)
    symnmf_labels = get_symnmf_clusters()
    symnmf_score = silhouette_samples(X, symnmf_clusters)
    kmeans_score = silhouette_samples(X, kmeans_clusters)
    return kmeans_score, symnmf_score

def get_points():
    return symnmf.read_input()[0]


def get_symnmf_clusters(H) -> list[int]:
    """
    uses the symnmf module to get H, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param H: The H matrix as a numpy array.
    @return: A list of cluster assignments for each data point.
    """
    H = symnmf.main()
    return np.argmax(H, axis=1).tolist()

if __name__ == "__main__":
    kmenas_score ,  symnmf_score = score_algos()
    print(f"nmf: {symnmf_score}")
    print(f"kmeans: {kmeans_score}")