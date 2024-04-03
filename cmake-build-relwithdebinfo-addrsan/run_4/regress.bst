=== BEST-OF-RUN ===
              generation: 13
                   nodes: 41
                   depth: 7
                    hits: 827
TOP INDIVIDUAL:



-- #1 --
                    hits: 827
             raw fitness: 827.0000
    standardized fitness: 827.0000
        adjusted fitness: 0.9988
TREE:
 (/ (* (log (exp (/ convex
                    (* area eccentricity)))) perimeter)
    (/ (* (exp (* (+ -0.85432 eccentricity)
                  (+ (* 0.71107 convex) area))) eccentricity)
       (- (/ (+ (exp (exp minor))
                (log (/ minor minor)))
             (* (/ 0.52671 convex)
                (- eccentricity eccentricity))) perimeter)))
TREE-equ:
y = ((log(exp((convex / (area * eccentricity)))) * perimeter) / ((exp(((-0.85432 + eccentricity) * ((0.71107 * convex) + area))) * eccentricity) / (((exp(exp(minor)) + log((minor / minor))) / ((0.52671 / convex) * (eccentricity - eccentricity))) - perimeter)))
