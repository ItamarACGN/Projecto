import sys
import pandas as pd
import numpy as np

# Constants
DEFAULT_ITER = 300
MAX_ITER = 1000
ALLOWED_FILES = {"txt", "csv"}


def file_to_array(path1, path2):
    # Check file extensions
    ext1 = path1.split(".")[-1]
    ext2 = path2.split(".")[-1]

    if ext1 in ALLOWED_FILES and ext2 in ALLOWED_FILES:
        # Read files without header (treat first row as data)
        data1 = pd.read_csv(path1, header=None)
        data2 = pd.read_csv(path2, header=None)

        # Inner Join using the first column (0) as the key
        # Source: Assignment 2.1.2
        data = pd.merge(data1, data2, how="inner", on=0)

        # Sort by key
        # Source: Assignment 2.1.3
        data.sort_values(by=0, inplace=True)

        # Save keys (indices) for output
        keys = data[0].to_numpy()

        # Drop the key column for clustering
        # Source: Assignment 2.5.9
        data.drop(0, axis=1, inplace=True)

        return keys, data.to_numpy()

    # General error for file issues
    print("An Error Has Occurred")
    sys.exit()


def init_centroids(points, N, k):
    """
    Implementation of K-means++ initialization.
    Returns the INDICES of the chosen centroids in the points array.
    """
    # 1. Choose first center uniformly at random
    first_idx = np.random.choice(N)
    chosen_indices = [first_idx]

    # Initialize distances (D(x)) based on the first centroid
    min_distances = np.linalg.norm(points - points[first_idx], axis=1)

    # 2. Loop to choose the remaining k-1 centers
    while len(chosen_indices) < k:
        # Calculate probabilities P(x) = D(x) / sum(D(x))
        dist_sum = np.sum(min_distances)
        probs = min_distances / dist_sum

        # Choose next center based on weights
        next_idx = np.random.choice(N, p=probs)
        chosen_indices.append(next_idx)

        # Update minimal distances for next iteration:
        # D(x) = min(current_D(x), distance_to_new_center)
        dist_to_new = np.linalg.norm(points - points[next_idx], axis=1)
        min_distances = np.minimum(min_distances, dist_to_new)

    return chosen_indices


def process_input():
    # 1. Argument Count Check
    if len(sys.argv) not in [5, 6]:
        print("An Error Has Occurred")
        sys.exit()

    # 2. Assign variables based on input length
    if len(sys.argv) == 6:
        # Format: script, K, iter, eps, file1, file2
        str_k = sys.argv[1]
        str_iter = sys.argv[2]
        str_eps = sys.argv[3]
        file_1 = sys.argv[4]
        file_2 = sys.argv[5]
    else:
        # Format: script, K, eps, file1, file2 (iter is default)
        str_k = sys.argv[1]
        str_iter = str(DEFAULT_ITER)
        str_eps = sys.argv[2]
        file_1 = sys.argv[3]
        file_2 = sys.argv[4]

    # Validate Iteration
    if str_iter.isdigit():
        iter_val = int(str_iter)
        if not (1 < iter_val < MAX_ITER):
            print("Incorrect maximum iteration!")
            sys.exit()
    else:
        print("Incorrect maximum iteration!")
        sys.exit()

    # Validate Epsilon (float >= 0)
    # Simple check without try-except block as requested
    is_float = str_eps.replace(".", "", 1).isdigit()
    if is_float:
        eps_val = float(str_eps)
        if eps_val < 0:
            print("Incorrect epsilon!")
            sys.exit()
    else:
        print("Incorrect epsilon!")
        sys.exit()

    # Read Files
    keys, points = file_to_array(file_1, file_2)
    N = len(points)

    # Validate K
    if str_k.isdigit():
        k_val = int(str_k)
        if not (1 < k_val < N):
            print("Incorrect number of clusters!")
            sys.exit()
    else:
        print("Incorrect number of clusters!")
        sys.exit()

    return points, keys, k_val, iter_val, eps_val, N


if __name__ == "__main__":
    # Set random seed as required
    np.random.seed(1234)  #

    # Process Input
    points, keys, k, iter_val, eps, N = process_input()

    # Run K-means++ Initialization
    chosen_indices = init_centroids(points, N, k)

    # Output 1: The indices of the initial centroids (mapped back to original keys)
    # Note: keys is a numpy array, keys[i] gets the original ID from the file
    chosen_keys = [str(int(keys[i])) for i in chosen_indices]
    print(",".join(chosen_keys))

    # TODO: Connect to C Module
    # 1. Get initial centroids: initial_centroids = points[chosen_indices]
    # 2. Import mykmeanssp
    # 3. final_centroids = mykmeanssp.fit(k, iter_val, eps, initial_centroids.tolist(), points.tolist())
    # 4. Print final_centroids
