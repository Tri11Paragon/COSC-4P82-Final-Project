=== BEST-OF-RUN ===
              generation: 14
                   nodes: 59
                   depth: 9
                    hits: 795
TOP INDIVIDUAL:



-- #1 --
                    hits: 795
             raw fitness: 795.0000
    standardized fitness: 795.0000
        adjusted fitness: 0.9987
TREE:
 (+ (- (* (+ (log (log area))
             (/ (exp major)
                (log major)))
          (* (* (/ eccentricity
                   (exp minor)) perimeter)
             (exp (- (* (exp extent) eccentricity) minor))))
       (* area perimeter))
    (* (- area
          (- (/ (/ (* major
                      (* major
                         (* major 0.32111)))
                   (* extent minor)) perimeter) area))
       (+ (- (- perimeter perimeter) area)
          (exp (/ (/ minor eccentricity) perimeter)))))
TREE-equ:
y = ((((log(log(area)) + (exp(major) / log(major))) * (((eccentricity / exp(minor)) * perimeter) * exp(((exp(extent) * eccentricity) - minor)))) - (area * perimeter)) + ((area - ((((major * (major * (major * 0.32111))) / (extent * minor)) / perimeter) - area)) * (((perimeter - perimeter) - area) + exp(((minor / eccentricity) / perimeter)))))
