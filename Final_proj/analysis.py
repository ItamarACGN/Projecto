import numpy as np
from sklearn.metrics import silhouette_samples

def get_clusters(H) -> list[int]:
    """
    Given the H matrix, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param H: The H matrix as a numpy array.
    @return: A list of cluster assignments for each data point.
    """
    return np.argmax(H, axis=1).tolist()
