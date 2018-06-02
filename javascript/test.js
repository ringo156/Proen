const jsonfile = "fdata.json"

function getQueryVars()
{
    // aray for return value
    var vars = [];
    // get query char and divide "&"
    var query_list = window.location.search.substring(1).split('&');
    // TEMP value for get value
    var temp_arr;
    //
    query_list.forEach(function(e, i, a){
        temp_arr = e.split('=');
        vars[ temp_arr[0] ] = temp_arr[1];
    })
    return vars;
}

function getURL(query_vars){
    if ("page" in query_vars){//page is existence
        var page = query_vars["page"];
        if(page === void 0){//page is null
            attrURL(0);
        }
        else{
            attrURL(page);
        }
    }
    else{// page is not existence
        attrURL(0);
    }
}

function readJSON(){
    return $.ajax({
        type: "get",
        url: jsonfile,
        dataType: "json",
    })
}

function attrURL(page){
    readJSON().done(function(result){
        console.log(result.length);
        if(page > result.length){
            // add serch dir
            var url = result[result.length-1].title + ".html";
        }
        else{
            var url = result[page].title + ".html";
        }
        console.log(url);
        $('#frameSample').attr('src', url);
    }).fail(function(result) {
        console.log(result);
    });
}
