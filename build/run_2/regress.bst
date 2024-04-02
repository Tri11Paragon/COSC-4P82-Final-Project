=== BEST-OF-RUN ===
              generation: 7
                   nodes: 16
                   depth: 7
                    hits: 833
TOP INDIVIDUAL:



-- #1 --
                    hits: 833
             raw fitness: 833.0000
    standardized fitness: 833.0000
        adjusted fitness: 0.9988
TREE:
 (/ (exp major)
    (log (exp (* major
                 (/ major
                    (+ (* -0.60973 minor)
                       (/ extent major)))))))
TREE-equ:
y = (exp(major) / log(exp((major * (major / ((-0.60973 * minor) + (extent / major)))))))
