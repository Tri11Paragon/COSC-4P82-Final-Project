=== BEST-OF-RUN ===
              generation: 4
                   nodes: 25
                   depth: 8
                    hits: 653
TOP INDIVIDUAL:



-- #1 --
                    hits: 653
             raw fitness: 653.0000
    standardized fitness: 653.0000
        adjusted fitness: 0.9985
TREE:
 (log (/ (/ (log area)
            (exp (exp (log (+ major perimeter)))))
         (/ (log (* perimeter
                    (* (exp (log extent)) convex)))
            (* (* minor convex) eccentricity))))
TREE-equ:
y = log(((log(area) / exp(exp(log((major + perimeter))))) / (log((perimeter * (exp(log(extent)) * convex))) / ((minor * convex) * eccentricity))))
