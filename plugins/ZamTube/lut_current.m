1;

function I = ip (vgk, vpk)
  mu=103.2
  kx=1.26
  kg1=446.0
  kp=3.4
  kvb=300.0
  e1=vpk .* log(1. + exp(kp .* (1.0/mu + vgk ./ (kvb + vpk .* vpk) .^ 0.5))) / kp
  if e1 < 0
    I = 0
    return
  endif
  I = double((e1 .^ kx)/kg1)
endfunction

gridsizeg = 2001
gridsizep = 2001
vgk = linspace(-20, 20, gridsizeg)
vpk = linspace(0, 500, gridsizep)
[vg, vp] = ndgrid(double(vgk), double(vpk))
cur = 1e+6*ip(double(vg), double(vp))
surf(vg,vp,cur)

more off
for i=1:gridsizeg
  for j=1:gridsizep
    fprintf("%f,\n", cur(i,j))
  endfor
endfor
