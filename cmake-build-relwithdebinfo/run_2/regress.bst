=== BEST-OF-RUN ===
              generation: 0
                   nodes: 42
                   depth: 5
                    hits: 793
TOP INDIVIDUAL:



-- #1 --
                    hits: 793
             raw fitness: 793.0000
    standardized fitness: 793.0000
        adjusted fitness: 0.9987
TREE:
 (- (/ (/ (- (log -0.65696)
             (log eccentricity))
          (+ (log major)
             (exp major)))
       (/ (/ (log area)
             (* -0.47770 major))
          (+ (exp major)
             (+ extent perimeter))))
    (+ (/ (exp (/ minor extent))
          (+ (* 0.92538 major)
             (exp major)))
       (exp (exp (log -0.99120)))))
TREE-equ:
y = ((((log(-0.65696) - log(eccentricity)) / (log(major) + exp(major))) / ((log(area) / (-0.47770 * major)) / (exp(major) + (extent + perimeter)))) - ((exp((minor / extent)) / ((0.92538 * major) + exp(major))) + exp(exp(log(-0.99120)))))
