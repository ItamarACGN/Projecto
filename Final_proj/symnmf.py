import numpy as np
from sys import argv, stdout
import symnmfmodule as symnmf


MAX_ITER = 300
EPSILON = 1e-4
GOALS = {"symnmf", "ddg", "sym", "norm"}
ERROR_MSG = "An Error Has Occurred"


def print_error_and_exit():
    print(ERROR_MSG)
    exit(1)


def file_to_matrix(file_name: str):
    """
    Reads The input file name and converts the data in it to a numpy array if valid.

    :param file_name: the input file to read
    :type file_name: str
    """
    try:
        file = open(file_name, "r")
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
    reads all the input args interprets them and returns the parameters for the algorithm.

    :returns:
        * **data** - a matrix of the given data points
        * **N** - the number of data points
        * **k_val** - the number of clusters,
        * **d** - dimensions,
        * **goal** -  the function to run according to the input goal.
    """
    # Validate input length
    if len(argv) != 4:
        print_error_and_exit()

    # Assign variables based on input length
    raw_k = argv[1]
    raw_goal = argv[2]
    raw_file = argv[3]

    # Validate Goal
    if raw_goal not in GOALS:
        exit(1)

    # file reading using helper function
    data, N, d = file_to_matrix(raw_file)
    if data is None:
        # file_to_matrix already printed a more specific message when possible
        exit(1)

    # Validate K - needs to be positive int less than N
    if not raw_k.isdigit() or int(raw_k) <= 0 or int(raw_k) >= N:
        # print(f"{line}")
        exit(1)
    k_val = int(raw_k)
    return data, N, k_val, d, raw_goal


def init_H(W: NDArray[float64], n: int, k: int):
    """
    Initializes the H (n by k) matrix randomly according to instructions.
    ( each value is chosen uniformly at random from the range [0, 2*sqrt(m/k)] where m is the mean of all elements in W).
    :param: W: The innormalized similarity matrix.
    :param: n: The number of rows in W.
    :param: k: The number of clusters.
    """
    np.random.seed(1234)
    m = np.mean(W)  # Compute the mean of all elements in W
    H = np.random.uniform(0, 2 * np.sqrt(m / k), size=(n, k))
    return H


def execute_goal(
    data: list[list[float]],
    N: int,
    k_val: int,
    d: int,
    goal: str,
    max_iter: int = MAX_ITER,
    epsilon: float = EPSILON,
):
    """
    Executes the given goal using the symnmf C API.
    @param data: The input data as a python list of lists.
    @param N: The number of data points.
    @param k_val: The number of clusters.
    @param d: The number of dimensions.
    @param goal: The goal to execute (one of "symnmf", "ddg", "sym", "norm").
    @return: The result of the executed goal as a numpy array or None if an error occurred.
    """
    if goal == "symnmf":
        # computes the normalized similarity matrix using the costume c module function
        W = np.array(symnmf.execute_goal(data, N, d, "norm"))
        H = init_H(W, N, k_val)
        # runs the symnmf function from the c module
        result = symnmf.optimize_H(H.tolist(), W.tolist(), max_iter, epsilon)

    else:
        # for any other goal we just use execute goal from the module
        result = symnmf.execute_goal(data, N, d, goal)
    return np.array(result)


def main():
    """
    main logic for the symnmf algo computes the given goal if input is valid
    if the input is not valid, it prints an error message and exits
    """
    input_data = read_input()
    if input_data is not None:
        # print( input_data)
        data, N, k_val, d, goal = input_data
        result = execute_goal(data.tolist(), N, k_val, d, goal)
        return result


if __name__ == "__main__":
    result = main()
    if result is not None:
        # if we have a result we print it in the asked format
        np.savetxt(stdout, result, fmt="%.4f", delimiter=",")
