function httpGetPrediction() {
    const json_data = {
        "annual_inc": document.getElementById("annual_inc").value,  
        "funded_amnt": document.getElementById("funded_amnt").value,  
        "funded_amnt_inv": document.getElementById("funded_amnt").value,
        "installment": document.getElementById("annual_inc").value,  
        "int_rate": document.getElementById("int_rate").value
    };
    
    const Http = new XMLHttpRequest();
    const url="http://localhost:5000/json";
    Http.open("POST", url);
    Http.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    Http.send(  JSON.stringify(json_data) );
    Http.onreadystatechange = (e) => {
        document.getElementById("result_name").innerHTML = "Result";
        document.getElementById("result").innerHTML = Http.responseText;
    }
}