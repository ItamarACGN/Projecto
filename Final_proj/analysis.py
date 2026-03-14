import symnmf 
import kmeans
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

def run_algos():
    data, N, k_val, d = read_input()
    data = data.tolist()
    H = symnmf.execute_goal(data,N,k_val,d,"symnmf")
    symnmf_clusters = get_clusters(H) 

    symnmf_score = silhouette_samples(data, symnmf_clusters)
    print(symnmf_score)


def get_clusters(H) -> list[int]:
    """
    Given the H matrix, returns a list of cluster assignments for each data point.
    Each entry in the returned list corresponds to a data point and contains the index of the cluster it belongs to (0-based).
    @param H: The H matrix as a numpy array.
    @return: A list of cluster assignments for each data point.
    """
    return np.argmax(H, axis=1).tolist()

run_algos()