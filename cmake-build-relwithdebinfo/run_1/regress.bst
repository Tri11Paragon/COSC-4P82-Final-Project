=== BEST-OF-RUN ===
              generation: 8
                   nodes: 51
                   depth: 6
                    hits: 749
TOP INDIVIDUAL:



-- #1 --
                    hits: 749
             raw fitness: 749.0000
    standardized fitness: 749.0000
        adjusted fitness: 0.9987
TREE:
 (+ (+ (/ (- perimeter area)
          (- (/ major eccentricity)
             (/ eccentricity extent)))
       (- (exp (exp (log extent)))
          (* (+ eccentricity minor)
             (- minor -0.86362))))
    (- (/ (+ (- area extent)
             (exp eccentricity))
          (log (log perimeter)))
       (- (* (/ eccentricity major)
             (/ extent major))
          (* (* perimeter eccentricity)
             (/ extent 0.46162)))))
TREE-equ:
y = ((((perimeter - area) / ((major / eccentricity) - (eccentricity / extent))) + (exp(exp(log(extent))) - ((eccentricity + minor) * (minor - -0.86362)))) + ((((area - extent) + exp(eccentricity)) / log(log(perimeter))) - (((eccentricity / major) * (extent / major)) - ((perimeter * eccentricity) * (extent / 0.46162)))))
