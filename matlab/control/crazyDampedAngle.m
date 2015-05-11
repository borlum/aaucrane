 clear;
%close all;

run('../model/cranemodel');

% Angle control, as damped as possible!

CV = 5;

Vcl = feedback(CV * Mx * Grx * rrx, 1);

Ctheta = 163 * (s + 6) * 1/(s+20);

Ctheta = (s + 6) * 1/(s+20);

Ctheta = 8.84*(s+6 ); 

dCtheta = c2d(Ctheta, .01, 'tustin');

Acl = feedback(Ctheta * Vcl * 1/s * -W,1);

C3 = 1.2;

%fprintf('Design procedure findes i kommentarerne \n');

% Først vælges et gain til det indre hastigheds loop. Baseret på rlocus(Mx * Grx * rrx) er et fornuftigt gain C1 = 5

% Nu laves vinkelkontrollen. rlocus(Vcl * 1/s * -W) giver anledning til at placere et nulpunkt i 6. rlocus((s+6) * Vcl * 1/s * -W) viser at et gain på 8 nok er ganske OK.

% Dette implementeres i Simulink, og vi simulerer med en 3 sek lang 10 V puls som forstyrrelse efter C1. Vi ser at det giver en fin dæmpning af vinklen. 
% Et kig på positionen viser at vinkel kontrollen giver et meget langvarigt bidrag, så for at sænke settling tiden indføres et I led.
% Hvis vi placerer en pol ekstra til venstre for krydset i rlocus((s+6) * Vcl * 1/s * -W) vil det få de to poler på den komplekse akse til at skyde i vejret, men til gengæld vil vi få en hurtigere settle time forde vi introducerer I. 
% Polen lægges i (s+20);
% rlocus((s+6)/(s+20) * Vcl * 1/s * -W) viser at vi får maksimal dæmpning ved et gain på 80, så det vælges. Simuleringen viser en nærmest instant settling på 3 sek pulsen.
% Input til motoren har rimelig størrelse.

% C2 er diskretiseret og implementeret på kranen vha den inverse z-transform. En ret elegant verificering af den fundne model kan findes i simulink filen.

% Controlleren er testet på kranen, og selvom magnituden af vinklen ikke helt passer med simu, så er forløbet magen til, og virkelig hurtigt. :) 

% Designet af positions-kontrol er lavet ud fra rlocus(feedback(Vcl * 1/s, -W * C2). 
% Det er en ret forvirrende rlocus, men en p factor på omkring 1 eller mindre virker sane. PI er ikke undersøgt, planten er efterhånden ret kompleks..
