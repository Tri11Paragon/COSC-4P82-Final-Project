=== BEST-OF-RUN ===
              generation: 0
                   nodes: 37
                   depth: 5
                    hits: 675
TOP INDIVIDUAL:



-- #1 --
                    hits: 675
             raw fitness: 675.0000
    standardized fitness: 675.0000
        adjusted fitness: 0.9985
TREE:
 (* (log (* (- (exp extent)
               (* convex convex))
            (+ (/ area perimeter)
               (- minor minor))))
    (- (log (exp (+ perimeter major)))
       (/ (* (* extent perimeter)
             (* minor perimeter))
          (+ (+ eccentricity extent)
             (+ major perimeter)))))
TREE-equ:
y = (log(((exp(extent) - (convex * convex)) * ((area / perimeter) + (minor - minor)))) * (log(exp((perimeter + major))) - (((extent * perimeter) * (minor * perimeter)) / ((eccentricity + extent) + (major + perimeter)))))
