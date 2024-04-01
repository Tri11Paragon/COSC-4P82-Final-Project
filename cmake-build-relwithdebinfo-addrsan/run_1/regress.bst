=== BEST-OF-RUN ===
              generation: 0
                   nodes: 51
                   depth: 5
                    hits: 750
TOP INDIVIDUAL:



-- #1 --
                    hits: 750
             raw fitness: 750.0000
    standardized fitness: 750.0000
        adjusted fitness: 0.9987
TREE:
 (- (+ (* (* (+ eccentricity -0.89684)
             (exp -0.00334))
          (exp (+ eccentricity minor)))
       (- (* (/ area area)
             (+ area perimeter))
          (/ (- major extent)
             (log minor))))
    (+ (/ (log (+ extent extent))
          (- (/ major eccentricity)
             (- perimeter perimeter)))
       (* (* (- eccentricity major)
             (+ minor eccentricity))
          (log (log -0.66041)))))
TREE-equ:
y = (((((eccentricity + -0.89684) * exp(-0.00334)) * exp((eccentricity + minor))) + (((area / area) * (area + perimeter)) - ((major - extent) / log(minor)))) - ((log((extent + extent)) / ((major / eccentricity) - (perimeter - perimeter))) + (((eccentricity - major) * (minor + eccentricity)) * log(log(-0.66041)))))
