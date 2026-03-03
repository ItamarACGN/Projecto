import numpy as np
from sys import argv

MAX_ITER = 300
EPSILON = 1e-4
GOALS = {"symnmf", "ddg", "sym", "norm"}

def file_to_matrix(file_name):
    """
    Reads The input file name and converts it to a numpy array if valid. 
    @param File: A file txt file object containing the input data.
    @return: A tuple containing the data as a numpy array, the number of rows, and the number of columns.
    """
    try:
        file = open(file_name, 'r')
        data = []
        N = 0
        d = None  # Store the expected dimension
        for line in file:
            line = line.strip()
            if not line:  # Skip empty lines
                continue
            row = [float(x) for x in line.split(",")]
            # Check that all vectors have the same length
            if d is None:
                d = len(row)
            elif len(row) != d:
                return None, None, None  # Vector length mismatch
            data.append(row)
            N += 1
        file.close()
        return np.array(data), N, d
    except:
        return None, None, None


def read_input():
    """
    Proccessing the cmd arrgs and validating them. 
    If the input is not valid we print "An Error Has Occurred" and return None.
    @ret data - a numpy array of the points for, N - the number of points,
    k_val - the number of clusters, d - dimensions,
    goal -  the function to run according to the input goal.
    """
    # 1. Validate input length
    if len(argv) != 4:
        print("An Error Has Occurred")
        return None

    # 2. Assign variables based on input length
    raw_k = argv[1]
    raw_goal = argv[2]
    raw_file = argv[3]

    # 3. Validate Goal
    if raw_goal not in GOALS:
        print("An Error Has Occurred")
        return None
    
    # 4. file reading using helper function
    data, N, d = file_to_matrix(open(raw_file, 'r'))
    if data is None:
            print("An Error Has Occurred")
            return None
    
    # 5. Validate K - nedds to be positive int less than N
    if not raw_k.isdigit() or int(raw_k) <= 0 or int(raw_k) >= N:
        print("An Error Has Occurred")
        return None
    k_val = int(raw_k)
    return data, N, k_val, MAX_ITER, EPSILON, d



    

def init_H(W, n, k):
    """
    Initializes the H (n by k) matrix with random values.
    @param W: The innormalized similarity matrix.
    @param n: The number of rows in W.
    @param k: The number of clusters.
    """
    np.random.seed(1234)  
    H = np.random.rand(W.shape[0], k)
