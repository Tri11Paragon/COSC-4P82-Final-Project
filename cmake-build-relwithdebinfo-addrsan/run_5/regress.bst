=== BEST-OF-RUN ===
              generation: 10
                   nodes: 35
                   depth: 7
                    hits: 799
TOP INDIVIDUAL:



-- #1 --
                    hits: 799
             raw fitness: 799.0000
    standardized fitness: 799.0000
        adjusted fitness: 0.9988
TREE:
 (+ (- perimeter
       (log (+ (exp minor)
               (- extent perimeter))))
    (* (+ (- major
             (/ eccentricity convex))
          (exp (/ eccentricity convex)))
       (/ (log (* perimeter perimeter))
          (* (log area)
             (log (/ (/ minor major) eccentricity))))))
TREE-equ:
y = ((perimeter - log((exp(minor) + (extent - perimeter)))) + (((major - (eccentricity / convex)) + exp((eccentricity / convex))) * (log((perimeter * perimeter)) / (log(area) * log(((minor / major) / eccentricity))))))
