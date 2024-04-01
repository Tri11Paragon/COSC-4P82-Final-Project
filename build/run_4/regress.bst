=== BEST-OF-RUN ===
              generation: 34
                   nodes: 28
                   depth: 7
                    hits: 909
TOP INDIVIDUAL:



-- #1 --
                    hits: 909
             raw fitness: 909.0000
    standardized fitness: 909.0000
        adjusted fitness: 0.9989
TREE:
 (* (/ 0.94014
       (* (* minor
             (exp (+ major minor)))
          (- (exp extent)
             (* perimeter
                (log (exp major))))))
    (* (/ (+ extent
             (/ perimeter major))
          (exp perimeter)) eccentricity))
TREE-equ:
y = ((0.94014 / ((minor * exp((major + minor))) * (exp(extent) - (perimeter * log(exp(major)))))) * (((extent + (perimeter / major)) / exp(perimeter)) * eccentricity))
