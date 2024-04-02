=== BEST-OF-RUN ===
              generation: 9
                   nodes: 46
                   depth: 6
                    hits: 898
TOP INDIVIDUAL:



-- #1 --
                    hits: 898
             raw fitness: 898.0000
    standardized fitness: 898.0000
        adjusted fitness: 0.9989
TREE:
 (- (* (/ eccentricity
          (- (exp (+ minor perimeter)) eccentricity))
       (* (/ (* (- minor perimeter)
                (/ perimeter major))
             (exp (- eccentricity eccentricity)))
          (/ (/ (+ major major)
                (* area area))
             (+ (exp major)
                (- 0.80038 minor)))))
    (log (/ (log -0.49049)
            (exp (exp (+ -0.35161 convex))))))
TREE-equ:
y = (((eccentricity / (exp((minor + perimeter)) - eccentricity)) * ((((minor - perimeter) * (perimeter / major)) / exp((eccentricity - eccentricity))) * (((major + major) / (area * area)) / (exp(major) + (0.80038 - minor))))) - log((log(-0.49049) / exp(exp((-0.35161 + convex))))))
