=== BEST-OF-RUN ===
              generation: 24
                   nodes: 29
                   depth: 5
                    hits: 887
TOP INDIVIDUAL:



-- #1 --
                    hits: 887
             raw fitness: 887.0000
    standardized fitness: 887.0000
        adjusted fitness: 0.9989
TREE:
 (/ (/ (/ extent
          (- area
             (/ extent convex)))
       (/ (exp (- perimeter major))
          (exp (+ -0.86676 0.47741))))
    (/ (* (* (* perimeter major)
             (exp perimeter))
          (log extent)) area))
TREE-equ:
y = (((extent / (area - (extent / convex))) / (exp((perimeter - major)) / exp((-0.86676 + 0.47741)))) / ((((perimeter * major) * exp(perimeter)) * log(extent)) / area))
