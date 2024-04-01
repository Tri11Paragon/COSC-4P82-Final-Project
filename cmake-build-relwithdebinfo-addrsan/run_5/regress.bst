=== BEST-OF-RUN ===
              generation: 15
                   nodes: 35
                   depth: 6
                    hits: 859
TOP INDIVIDUAL:



-- #1 --
                    hits: 859
             raw fitness: 859.0000
    standardized fitness: 859.0000
        adjusted fitness: 0.9988
TREE:
 (/ (/ minor
       (* (/ (log convex)
             (log convex))
          (* (+ -0.05978
                (/ extent area)) 0.97681)))
    (exp (- (/ (- (log major) perimeter)
               (/ (* minor minor)
                  (+ eccentricity convex)))
            (/ major
               (+ perimeter perimeter)))))
TREE-equ:
y = ((minor / ((log(convex) / log(convex)) * ((-0.05978 + (extent / area)) * 0.97681))) / exp((((log(major) - perimeter) / ((minor * minor) / (eccentricity + convex))) - (major / (perimeter + perimeter)))))
