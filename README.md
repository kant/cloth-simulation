# Cloth Simulation

In our simulation, we treat a cloth as a mesh of particles held together by spring forces. In addition to spring forces, each particle is impacted by the forces of gravity, damping, and vicosity (wind). Force affects the velocity of each particle, which in turn affects its position.

Where a particle is at any time t, is affected by it's posiiton, velocity, and force at any previous time. We can generalize this problem as a numerical integration of ordinary differential equation. We solve it using euler's method.

## Computing Force
  For every step of t seconds, we compute total force as follows:

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}_{\text{total}}&space;=&space;{F}_{\text{spring}}&space;&plus;&space;{F}_{\text{gravity}}&plus;&space;{F}_{\text{viscous}}&plus;&space;{F}_{\text{damping}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}_{\text{total}}&space;=&space;{F}_{\text{spring}}&space;&plus;&space;{F}_{\text{gravity}}&plus;&space;{F}_{\text{viscous}}&plus;&space;{F}_{\text{damping}}" title="\mathbf{F}_{\text{total}} = {F}_{\text{spring}} + {F}_{\text{gravity}}+ {F}_{\text{viscous}}+ {F}_{\text{damping}}" /></a>

### Spring Force

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}_{\text{spring}}&space;=&space;K(L_0&space;-&space;\|&space;\mathrm{p}&space;-&space;\mathrm{q}&space;\|)&space;\frac{\mathrm{p}&space;-&space;\mathrm{q}}{\|&space;\mathrm{p}&space;-&space;\mathrm{q}&space;\|}." target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}_{\text{spring}}&space;=&space;K(L_0&space;-&space;\|&space;\mathrm{p}&space;-&space;\mathrm{q}&space;\|)&space;\frac{\mathrm{p}&space;-&space;\mathrm{q}}{\|&space;\mathrm{p}&space;-&space;\mathrm{q}&space;\|}." title="\mathbf{F}_{\text{spring}} = K(L_0 - \| \mathrm{p} - \mathrm{q} \|) \frac{\mathrm{p} - \mathrm{q}}{\| \mathrm{p} - \mathrm{q} \|}." /></a>

  <img src="https://latex.codecogs.com/gif.latex?p" /> and <img src="https://latex.codecogs.com/gif.latex?q" /> are the *positions* of two particles connected by spring forces

  <img src="https://latex.codecogs.com/gif.latex?L_0" /> is rest length (a constant describing initial distance between two particles when under the influence of no forces, this differs for each type of spring force)

  <img src="https://latex.codecogs.com/gif.latex?K" /> is stiffness (a constant, also differs for each type of spring force)

### Gravitational Force

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}_{\text{gravity}}&space;=&space;\begin{pmatrix}&space;0\\&space;-mg\\&space;0&space;\end{pmatrix}," target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}_{\text{gravity}}&space;=&space;\begin{pmatrix}&space;0\\&space;-mg\\&space;0&space;\end{pmatrix}," title="\mathbf{F}_{\text{gravity}} = \begin{pmatrix} 0\\ -mg\\ 0 \end{pmatrix}," /></a>

  <img src="https://latex.codecogs.com/gif.latex?m" /> is the mass of each particle

  <img src="https://latex.codecogs.com/gif.latex?g" /> is gravity
    
### Viscous (Wind) Force

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}_{\text{viscous}}&space;=&space;c_v&space;[\mathrm{n}&space;\cdot&space;(\mathrm{u}_\text{fluid}&space;-&space;\mathbf{v})]&space;\mathrm{n}," target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}_{\text{viscous}}&space;=&space;c_v&space;[\mathrm{n}&space;\cdot&space;(\mathrm{u}_\text{fluid}&space;-&space;\mathbf{v})]&space;\mathrm{n}," title="\mathbf{F}_{\text{viscous}} = c_v [\mathrm{n} \cdot (\mathrm{u}_\text{fluid} - \mathbf{v})] \mathrm{n}," /></a>

  <img src="https://latex.codecogs.com/gif.latex?n" /> is the normal of the current particle

  <img src="https://latex.codecogs.com/gif.latex?v" /> is the velocity of the current particle

  <img src="https://latex.codecogs.com/gif.latex?U_{fluid}" /> is the direction of the viscous force (think of wind)

  <img src="https://latex.codecogs.com/gif.latex?C_v" /> is a constant dictating the intensity of the viscous force (think of wind)

### Damping Force

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}_{\text{damp}}&space;=&space;-c_d\,\mathbf{v}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}_{\text{damp}}&space;=&space;-c_d\,\mathbf{v}" title="\mathbf{F}_{\text{damp}} = -c_d\,\mathbf{v}" /></a>

  <img src="https://latex.codecogs.com/gif.latex?v" /> is the current velocity of a particle

  <img src="https://latex.codecogs.com/gif.latex?C_d" /> is a constant dictating the intensity of the damping force
  
## Computing Velocity

  Velocity at any step <a href="https://www.codecogs.com/eqnedit.php?latex=\Delta&space;t" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\Delta&space;t" title="\Delta t" /></a> is computed as 

  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{v}_{current}&space;\gets&space;\mathbf{v}_{previous}&space;&plus;&space;\Delta&space;t\,\frac{\mathbf{F}_{total}}{m}." target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{v}_{current}&space;\gets&space;\mathbf{v}_{previous}&space;&plus;&space;\Delta&space;t\,\frac{\mathbf{F}_{total}}{m}." title="\mathbf{v}_{current} \gets \mathbf{v}_{previous} + \Delta t\,\frac{\mathbf{F}_{total}}{m}." /></a>
  
## Computing Position
 
  Position at any step <a href="https://www.codecogs.com/eqnedit.php?latex=\Delta&space;t" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\Delta&space;t" title="\Delta t" /></a> is computed as 
 
  <a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{x}_{current}&space;\gets&space;\mathbf{x}_{previous}&space;&plus;&space;\Delta&space;t&space;*&space;\mathbf{v}_{current}." target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{x}_{current}&space;\gets&space;\mathbf{x}_{previous}&space;&plus;&space;\Delta&space;t&space;*&space;\mathbf{v}_{current}." title="\mathbf{x}_{current} \gets \mathbf{x}_{previous} + \Delta t * \mathbf{v}_{current}." /></a>
