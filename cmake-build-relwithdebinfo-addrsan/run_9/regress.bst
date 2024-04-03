=== BEST-OF-RUN ===
              generation: 15
                   nodes: 30
                   depth: 5
                    hits: 889
TOP INDIVIDUAL:



-- #1 --
                    hits: 889
             raw fitness: 889.0000
    standardized fitness: 889.0000
        adjusted fitness: 0.9989
TREE:
 (/ (/ (/ (log major)
          (- area
             (/ extent convex)))
       (/ (exp (- perimeter major))
          (exp (+ -0.86676 0.47741))))
    (/ (* (* (* perimeter major)
             (exp perimeter))
          (log extent)) area))
TREE-equ:
y = (((log(major) / (area - (extent / convex))) / (exp((perimeter - major)) / exp((-0.86676 + 0.47741)))) / ((((perimeter * major) * exp(perimeter)) * log(extent)) / area))
