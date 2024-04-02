=== BEST-OF-RUN ===
              generation: 1
                   nodes: 37
                   depth: 5
                    hits: 586
TOP INDIVIDUAL:



-- #1 --
                    hits: 586
             raw fitness: 586.0000
    standardized fitness: 586.0000
        adjusted fitness: 0.9983
TREE:
 (+ (- (- (- (/ convex minor)
             (+ minor minor))
          (log (/ perimeter area)))
       (exp (log (log eccentricity))))
    (- (log (exp (- major convex)))
       (/ (+ (+ convex convex)
             (log convex))
          (* (+ area major)
             (log major)))))
TREE-equ:
y = (((((convex / minor) - (minor + minor)) - log((perimeter / area))) - exp(log(log(eccentricity)))) + (log(exp((major - convex))) - (((convex + convex) + log(convex)) / ((area + major) * log(major)))))
