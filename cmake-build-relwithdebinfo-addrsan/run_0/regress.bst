=== BEST-OF-RUN ===
              generation: 20
                   nodes: 29
                   depth: 7
                    hits: 873
TOP INDIVIDUAL:



-- #1 --
                    hits: 873
             raw fitness: 873.0000
    standardized fitness: 873.0000
        adjusted fitness: 0.9989
TREE:
 (/ (/ (+ minor
          (- (log minor)
             (* (exp extent)
                (+ minor
                   (+ minor eccentricity)))))
       (/ (* (exp major) eccentricity) major))
    (exp (- perimeter
            (+ eccentricity
               (- eccentricity minor)))))
TREE-equ:
y = (((minor + (log(minor) - (exp(extent) * (minor + (minor + eccentricity))))) / ((exp(major) * eccentricity) / major)) / exp((perimeter - (eccentricity + (eccentricity - minor)))))
