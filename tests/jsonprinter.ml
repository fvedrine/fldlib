
open Yojson
open Basic
open Util


let safe_to_string (r:json) (s:string) =
  try
    r |> member s |> to_string
  with
    _ ->
    begin
      to_file "err.txt" r;
      failwith ("An error occured while selecting field "^s^". See file err.txt\n")
    end

let safe_to_list (r:json) (s:string) =
  try
    r |> member s |> to_list
  with
    _ ->
    begin
      to_file "err.txt" r;
    failwith ("An error occured while selecting field "^s^". See file err.txt\n")
    end

let safe_to_float (r:json) (s:string) =
  try
    r |> member s |> to_float
  with
    _ ->
    begin
      to_file "err.txt" r;
      failwith ("An error occured while selecting field "^s^". See file err.txt\n")  
    end


(* printing utils *)

let print_dash fmt n =
  for i=1 to n do
    Format.fprintf fmt "-"
  done

let print_empty_space fmt n =
  for i=1 to n do
    Format.fprintf fmt " "
  done
            
let print_full_line sizes fmt ()=
  Array.iter
    (fun x -> Format.fprintf fmt "|"; print_dash fmt x)
    sizes;
  Format.fprintf fmt "|@."

let print_header sizes fmt header =
  Array.iteri
    (fun i x -> Format.fprintf fmt "|%-*s" sizes.(i) x)
    header;
  Format.fprintf fmt "|@."

let print_empty_columns sizes fmt j =
  Array.iteri
    (fun i x -> if i<=j then Format.fprintf fmt "|%a" print_empty_space x)
    sizes
      
(* assuming r is a record {"float_min":xx, "float_max":yy, "err_min":zz, etc} }*)
let print_interval sizes j (s:string) fmt (r:json) : unit =
  try
  let val_min : float = safe_to_float r (s^"_min") in
  let val_max : float = safe_to_float r (s^"_max") in
  let (interv:string) = Format.sprintf " [@[%g, %g@]] " val_min val_max in
  Format.fprintf fmt "%-*s" (sizes.(j)) interv
  with
    _ -> failwith "error in print_interval"

let print_tool sizes j fmt (r:json) : unit =
  try
  let tname = safe_to_string r "tname" in
  Format.fprintf fmt "| %-*s |%a|%a|%a|%a|"
                 (sizes.(j)-2) tname
                 (print_interval sizes (j+1) "float") r
                 (print_interval sizes (j+2) "err") r
                 (print_interval sizes (j+3) "rel") r
                 (print_interval sizes (j+4) "real") r
  with
    _ -> failwith "error in print_tool"

let print_variable sizes j fmt (r:json) : unit =
  try
  let vname = safe_to_string r "name" in
  let tool_list = safe_to_list r "tools" in
  Format.fprintf fmt "| %-*s " (sizes.(j)-2) vname;
  (* first tool *)
  Format.fprintf fmt "%a@." (print_tool sizes (j+1)) (List.hd tool_list);
  (* other tools *)
  List.iter
    (fun r ->
      print_empty_columns sizes fmt j;
      Format.fprintf fmt "%a@." (print_tool sizes (j+1))r)
    (List.tl tool_list)
  with
    _ -> failwith "error in print_variable"

let print_files sizes j fmt (r:json) : unit =
  try
  let dirname = safe_to_string r "dir" in
  let fname = safe_to_string r "file" in
  let var_list = safe_to_list r "variables" in
  begin
    Format.fprintf fmt "| %-*s | %-*s " (sizes.(j)-2) dirname (sizes.(j+1)-2) fname;
    (* first var *)
    print_variable sizes (j+2) fmt (List.hd var_list);
    (* other vars *)
    List.iter 
      (fun r ->
          print_empty_columns sizes fmt (j+1);
          print_variable sizes (j+2) fmt r
      )
      (List.tl var_list)
  end
  with
    _ -> failwith "error in print_files"


let compute_max_size_interval (res:int array) (j:int) (s:string) (r:json) =
  let val_min : float = safe_to_float r (s^"_min") in
  let val_max : float = safe_to_float r (s^"_max") in
  let str = Format.sprintf " [@[%g, %g@]] " val_min val_max in
  res.(j) <- max res.(j) ((String.length str))

let compute_max_size_tool (res:int array) (j:int) (r:json) =
  let tname = safe_to_string r "tname" in
  res.(j) <- max res.(j) (String.length tname+2);
  compute_max_size_interval res (j+1) "float" r;
  compute_max_size_interval res (j+2) "err" r;
  compute_max_size_interval res (j+3) "rel" r;
  compute_max_size_interval res (j+4) "real" r
            
let compute_max_size_variable (res:int array) (j:int) (r:json) =
  let vname = safe_to_string r "name" in
  let tool_list = safe_to_list r "tools" in
  res.(j) <- max res.(j) (String.length vname+2);
  List.iter (compute_max_size_tool res (j+1)) tool_list
             
let compute_max_size_files (res:int array) (j:int) (r:json) =
  let dirname = safe_to_string r "dir" in
  let fname = safe_to_string r "file" in
  let var_list = safe_to_list r "variables" in
  res.(j) <- max res.(j) (String.length dirname+2);
  res.(j+1) <- max res.(j+1) (String.length fname+2);
  List.iter (compute_max_size_variable res (j+2)) var_list


                  
let print_timestamp fmt (r:json) : unit =
  let time = safe_to_string r "time_stamp" in
  let results_list = safe_to_list r "content" in
  let header_array = [|" directory ";" file ";" variable ";" tool "; " domain(float) "; " absolute error "; " relative error "; " domain(real) " |] in
  let max_size_array = Array.map String.length header_array in
  List.iter (compute_max_size_files max_size_array 0) results_list;
  Format.fprintf fmt "time_stamp:%s@." time;
  print_full_line max_size_array fmt ();
  print_header max_size_array fmt header_array;
  print_full_line max_size_array fmt ();
  List.iter (print_files max_size_array 0 fmt) results_list;
  print_full_line max_size_array fmt ()
  
(* main function *)
let _ =
  let n = Array.length Sys.argv in
  let r =
  if n=1 then
  (*   Format.printf "Error: please provide a file name" *)
  (* else if n>2 then *)
    (*   Format.printf "Error: this programs accepts only one file name" *)
    begin
      from_channel stdin
    end
  else
    begin
      from_file Sys.argv.(1)
    
    end
  in
  print_timestamp Format.std_formatter r
