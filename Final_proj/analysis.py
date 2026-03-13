import symnmf 
import kmeans
import numpy as np
from sklearn.metrics import silhouette_samples

def run_algos():
    H = symnmf.main()
    symnmf_clusters = get_clusters(H) 
    kmeans_centroids = kmeans.main() 

    symnmf_score = silhouette_samples(get_points(), symnmf_clusters)

def get_points():
    return symnmf.read_input()[0]


def get_clusters(H) -> list[int]:
    """
    Given the H matrix, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param H: The H matrix as a numpy array.
    @return: A list of cluster assignments for each data point.
    """
    return np.argmax(H, axis=1).tolist()
