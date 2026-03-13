import symnmf 
import kmeans

def run_algos():
    H = symnmf.main()
    symnmf_res = clusters_from_h(H)
    kmeans_res = kmeans.main()

    return symnmf

def get_points():
    return symnmf.read_input()[0]