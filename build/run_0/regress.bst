=== BEST-OF-RUN ===
              generation: 7
                   nodes: 36
                   depth: 6
                    hits: 901
TOP INDIVIDUAL:



-- #1 --
                    hits: 901
             raw fitness: 901.0000
    standardized fitness: 901.0000
        adjusted fitness: 0.9989
TREE:
 (/ (exp minor)
    (* (/ (log (/ (/ convex minor)
                  (+ convex extent)))
          (* (+ (exp perimeter)
                (log minor))
             (+ (* extent extent)
                (exp minor))))
       (exp (/ (- (exp eccentricity)
                  (* convex major))
               (exp (log area))))))
TREE-equ:
y = (exp(minor) / ((log(((convex / minor) / (convex + extent))) / ((exp(perimeter) + log(minor)) * ((extent * extent) + exp(minor)))) * exp(((exp(eccentricity) - (convex * major)) / exp(log(area))))))
