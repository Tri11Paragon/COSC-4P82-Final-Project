=== BEST-OF-RUN ===
              generation: 45
                   nodes: 31
                   depth: 6
                    hits: 874
TOP INDIVIDUAL:



-- #1 --
                    hits: 874
             raw fitness: 874.0000
    standardized fitness: 874.0000
        adjusted fitness: 0.9989
TREE:
 (/ (+ (/ convex
          (/ perimeter perimeter))
       (- (log eccentricity)
          (* convex minor)))
    (- (exp (/ minor
               (- (/ minor area)
                  (log eccentricity))))
       (/ (exp perimeter)
          (/ eccentricity
             (/ extent area)))))
TREE-equ:
y = (((convex / (perimeter / perimeter)) + (log(eccentricity) - (convex * minor))) / (exp((minor / ((minor / area) - log(eccentricity)))) - (exp(perimeter) / (eccentricity / (extent / area)))))
