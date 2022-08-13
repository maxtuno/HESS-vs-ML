# HESS Algorithm v/s ML

1 - Maximum Margin Classifier (fast/weak) hess-mmc.cc

The maximal margin classifier is the optimal hyperplane defined in the (rare) case where two classes are linearly separable. 

    usage : hesxs-mmc data.txt labels.txt percent_of_total_entries

    ex:
    python3 generator
    ./hess-mmc data.txt labels.txt 0.0001
    ./hess-mmc data.txt labels.txt 0.0 (finish if is separable or finish all iterations)