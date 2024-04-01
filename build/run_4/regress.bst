=== BEST-OF-RUN ===
              generation: 2
                   nodes: 37
                   depth: 6
                    hits: 796
TOP INDIVIDUAL:



-- #1 --
                    hits: 796
             raw fitness: 796.0000
    standardized fitness: 796.0000
        adjusted fitness: 0.9987
TREE:
 (/ (log (exp (log (- (/ convex perimeter)
                      (- extent -0.49143)))))
    (* (* (- (+ extent perimeter)
             (* convex extent))
          (+ (exp eccentricity)
             (exp major)))
       (* (+ (- major 0.27873)
             (exp minor))
          (* (exp perimeter)
             (log perimeter)))))
TREE-equ:
y = (log(exp(log(((convex / perimeter) - (extent - -0.49143))))) / ((((extent + perimeter) - (convex * extent)) * (exp(eccentricity) + exp(major))) * (((major - 0.27873) + exp(minor)) * (exp(perimeter) * log(perimeter)))))
