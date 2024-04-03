=== BEST-OF-RUN ===
              generation: 9
                   nodes: 39
                   depth: 6
                    hits: 724
TOP INDIVIDUAL:



-- #1 --
                    hits: 724
             raw fitness: 724.0000
    standardized fitness: 724.0000
        adjusted fitness: 0.9986
TREE:
 (/ (log (+ (/ (log major)
               (+ (+ eccentricity extent)
                  (log -0.61570)))
            (exp (* (- convex eccentricity)
                    (+ extent eccentricity)))))
    (/ convex
       (* (exp (- (/ perimeter extent)
                  (/ -0.39868 convex)))
          (* (log extent)
             (/ (/ area convex) eccentricity)))))
TREE-equ:
y = (log(((log(major) / ((eccentricity + extent) + log(-0.61570))) + exp(((convex - eccentricity) * (extent + eccentricity))))) / (convex / (exp(((perimeter / extent) - (-0.39868 / convex))) * (log(extent) * ((area / convex) / eccentricity)))))
