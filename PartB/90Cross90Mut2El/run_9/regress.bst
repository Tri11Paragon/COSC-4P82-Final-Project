=== BEST-OF-RUN ===
              generation: 38
                   nodes: 26
                   depth: 8
                    hits: 927
TOP INDIVIDUAL:



-- #1 --
                    hits: 927
             raw fitness: 927.0000
    standardized fitness: 927.0000
        adjusted fitness: 0.9989
TREE:
 (/ area
    (+ (/ (/ perimeter
             (exp (* (log eccentricity) minor)))
          (/ (/ 0.25061
                (exp (/ (- major 0.42418)
                        (/ -0.24796 -0.89867))))
             (log (log eccentricity)))) convex))
TREE-equ:
y = (area / (((perimeter / exp((log(eccentricity) * minor))) / ((0.25061 / exp(((major - 0.42418) / (-0.24796 / -0.89867)))) / log(log(eccentricity)))) + convex))
