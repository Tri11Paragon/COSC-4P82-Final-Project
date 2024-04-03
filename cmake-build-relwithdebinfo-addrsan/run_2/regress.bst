=== BEST-OF-RUN ===
              generation: 5
                   nodes: 48
                   depth: 8
                    hits: 725
TOP INDIVIDUAL:



-- #1 --
                    hits: 725
             raw fitness: 725.0000
    standardized fitness: 725.0000
        adjusted fitness: 0.9986
TREE:
 (- (/ (- extent
          (- (exp (- perimeter 0.51575))
             (log (log minor))))
       (exp (- (* (+ minor perimeter)
                  (log extent))
               (+ (+ (/ extent
                        (+ area major)) perimeter)
                  (/ eccentricity
                     (/ area area))))))
    (/ (* (* major
             (- extent extent)) extent)
       (/ perimeter
          (/ (- eccentricity convex) eccentricity))))
TREE-equ:
y = (((extent - (exp((perimeter - 0.51575)) - log(log(minor)))) / exp((((minor + perimeter) * log(extent)) - (((extent / (area + major)) + perimeter) + (eccentricity / (area / area)))))) - (((major * (extent - extent)) * extent) / (perimeter / ((eccentricity - convex) / eccentricity))))
