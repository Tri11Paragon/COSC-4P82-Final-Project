=== BEST-OF-RUN ===
              generation: 4
                   nodes: 38
                   depth: 7
                    hits: 864
TOP INDIVIDUAL:



-- #1 --
                    hits: 864
             raw fitness: 864.0000
    standardized fitness: 864.0000
        adjusted fitness: 0.9988
TREE:
 (/ (- (+ extent
          (/ (* 0.30748 major)
             (+ convex convex)))
       (exp (+ (log area)
               (log convex))))
    (log (+ (- (exp minor)
               (/ eccentricity major))
            (* (exp (- perimeter major))
               (* (/ (exp perimeter) minor)
                  (* area eccentricity))))))
TREE-equ:
y = (((extent + ((0.30748 * major) / (convex + convex))) - exp((log(area) + log(convex)))) / log(((exp(minor) - (eccentricity / major)) + (exp((perimeter - major)) * ((exp(perimeter) / minor) * (area * eccentricity))))))
