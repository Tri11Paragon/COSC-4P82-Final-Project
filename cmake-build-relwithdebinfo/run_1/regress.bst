=== BEST-OF-RUN ===
              generation: 0
                   nodes: 20
                   depth: 4
                    hits: 572
TOP INDIVIDUAL:



-- #1 --
                    hits: 572
             raw fitness: 572.0000
    standardized fitness: 572.0000
        adjusted fitness: 0.9983
TREE:
 (/ (exp (- (- area major)
            (+ area perimeter)))
    (* (log (exp perimeter))
       (- (- -0.74710 minor)
          (+ perimeter extent))))
TREE-equ:
y = (exp(((area - major) - (area + perimeter))) / (log(exp(perimeter)) * ((-0.74710 - minor) - (perimeter + extent))))
