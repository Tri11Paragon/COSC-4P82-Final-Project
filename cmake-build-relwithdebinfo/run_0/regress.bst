=== BEST-OF-RUN ===
              generation: 46
                   nodes: 54
                   depth: 7
                    hits: 848
TOP INDIVIDUAL:



-- #1 --
                    hits: 848
             raw fitness: 848.0000
    standardized fitness: 848.0000
        adjusted fitness: 0.9988
TREE:
 (- (/ (+ (exp (+ (+ minor -0.32764)
                  (/ perimeter major)))
          (+ minor
             (* (* area eccentricity)
                (exp eccentricity))))
       (- extent eccentricity))
    (/ (* 0.22920
          (log 0.59994))
       (* (/ (* (+ minor convex)
                (* minor
                   (+ minor eccentricity)))
             (exp (* 0.69819 major)))
          (* (* (+ (log minor)
                   (+ major convex)) minor)
             (- convex major)))))
TREE-equ:
y = (((exp(((minor + -0.32764) + (perimeter / major))) + (minor + ((area * eccentricity) * exp(eccentricity)))) / (extent - eccentricity)) - ((0.22920 * log(0.59994)) / ((((minor + convex) * (minor * (minor + eccentricity))) / exp((0.69819 * major))) * (((log(minor) + (major + convex)) * minor) * (convex - major)))))
