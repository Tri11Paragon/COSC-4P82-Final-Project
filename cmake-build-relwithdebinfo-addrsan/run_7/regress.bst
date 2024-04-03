=== BEST-OF-RUN ===
              generation: 0
                   nodes: 65
                   depth: 6
                    hits: 784
TOP INDIVIDUAL:



-- #1 --
                    hits: 784
             raw fitness: 784.0000
    standardized fitness: 784.0000
        adjusted fitness: 0.9987
TREE:
 (- (/ (* (log (* (* extent extent)
                  (- eccentricity area)))
          (log (log (- 0.62511 -0.12697))))
       (log (exp (* (log convex)
                    (* minor eccentricity)))))
    (/ (log (* (/ (* extent perimeter)
                  (log eccentricity))
               (/ (/ eccentricity perimeter)
                  (* -0.49554 convex))))
       (* (+ (exp (* convex convex))
             (+ (- -0.22595 convex)
                (/ convex area)))
          (/ (/ (exp convex)
                (/ convex eccentricity))
             (- (exp perimeter)
                (log perimeter))))))
TREE-equ:
y = (((log(((extent * extent) * (eccentricity - area))) * log(log((0.62511 - -0.12697)))) / log(exp((log(convex) * (minor * eccentricity))))) - (log((((extent * perimeter) / log(eccentricity)) * ((eccentricity / perimeter) / (-0.49554 * convex)))) / ((exp((convex * convex)) + ((-0.22595 - convex) + (convex / area))) * ((exp(convex) / (convex / eccentricity)) / (exp(perimeter) - log(perimeter))))))
