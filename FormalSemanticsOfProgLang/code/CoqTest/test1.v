Inductive day : Type :=
    | Monday
    | Tuesday
    | Wednesday
    | Thursday
    | Friday
    | Saturday
    | Sunday.

Definition next_weekday (d:day) : day :=
    match d with
    | Monday => Tuesday
    | Tuesday => Wednesday
    | Wednesday => Thursday
    | Thursday => Friday
    | Friday => Saturday
    | Saturday => Monday
    | Sunday => Monday
    end.

Compute (next_weekday Friday).
(* 
= Saturday
: day
*)

Compute (next_weekday (next_weekday Saturday)).
(*
= Tuesday
: day
*)
