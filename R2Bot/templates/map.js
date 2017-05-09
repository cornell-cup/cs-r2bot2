function zeros(dimensions) {
    var array = [];

    for (var i = 0; i < dimensions[0]; ++i) {
        array.push(dimensions.length == 1 ? 0 : zeros(dimensions.slice(1)));
    }

    array[15][15] = 1;

    return array;
}

var canvas1 = document.getElementById('main');
var mctx = canvas1.getContext('2d');
var dict = {};
function drawMap(dist) {
    var map = zeros([30, 30]);
    mctx.clearRect(0, 0, 301, 301);
    //splitting up array of sensor data and creating dictionary of sensor:sensorData
    var n = dist.split("|");
    //console.log(n);

    for (var i = 0; i < n.length; i++) {
        var split = n[i].split(',');
        console.log(n[i]);
        if (parseFloat(split[1]) <= 20.0 && parseFloat(split[1]) > 4) {
            dict[split[0]] = parseFloat(split[1]) / 2;
        }
    }
    console.log(dict);
    //storing values in sensor and distance 
    for (var key in dict) {
        var sensor = key;
        var distance = dict[key];
        angle = parseFloat(getAngle(sensor)) * Math.PI / 180;
        findEdge(distance, angle, sensor);
    }

    for (var i = 0; i < map.length; i++) {
        for (var j = 0; j < map[i].length; j++) {
            if (map[i][j] === 1) {
                mctx.fillStyle = 'Red';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 2) {
                mctx.fillStyle = 'Yellow';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 3) {
                mctx.fillStyle = 'Cyan';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 4) {
                mctx.fillStyle = 'Green';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 5) {
                mctx.fillStyle = 'DarkBlue';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 6) {
                mctx.fillStyle = 'Pink';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 7) {
                mctx.fillStyle = 'Aquamarine';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 8) {
                mctx.fillStyle = 'Beige';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 9) {
                mctx.fillStyle = 'Coral';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 10) {
                mctx.fillStyle = 'Crimson';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 11) {
                mctx.fillStyle = 'MediumOrchid';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 12) {
                mctx.fillStyle = 'PaleVioletRed';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 13) {
                mctx.fillStyle = 'Tomato';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 14) {
                mctx.fillStyle = 'SkyBlue';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }

        }
    }

    function findEdge(distance, displacement, sensor) {
        //R2 position = map[15][15]

        var centerAngle = 90;
        for (var i = 0; i < 4; i++) { //going counterclockwise
            var xComponent = Math.floor(distance * Math.cos(centerAngle));
            var yComponent = Math.floor(distance * Math.sin(centerAngle));
            map[15 - yComponent + displacmement[1]][15 + xComponent + displacement[0]] = getColor(sensor);
            centerAngle += 5 * Math.PI / 180;
        }
        var angle = 90;
        for (var i = 0; i < 4; i++) { //going clockwise
            var xComponent = Math.floor(distance * Math.cos(angle));
            var yComponent = Math.floor(distance * Math.sin(angle));
            map[15 - yComponent + displacmement[1]][15 + xComponent + displacement[0]] = getColor(sensor);
            angle -= 5 * Math.PI / 180;
        }
        //column, row
    }
    function getColor(sensor) {
        var color = 0;
        switch (sensor) {
            case "U1SENSOR":
                color = 2;
                break;
            case "U2SENSOR":
                color = 3;
                break;
            case "U3SENSOR":
                color = 4;
                break;
            case "U4SENSOR":
                color = 5;
                break;
            case "U5SENSOR":
                color = 6;
                break;
            case "U6SENSOR":
                color = 7;
                break;
            case "U7SENSOR":
                color = 8;
                break;
            case "U8SENSOR":
                color = 9;
                break;
            case "U9SENSOR":
                color = 10;
                break;
            case "U10SENSOR":
                color = 11;
                break;
            case "U11SENSOR":
                color = 12;
                break;
            case "U12SENSOR":
                color = 13;
                break;
            case "U13SENSOR":
                color = 14;
                break;
            case "U14SENSOR":
                color = 15;
                break;
            }

        return color;
    }
    function getAngle(sensor) {
        var displacement = [];
        switch (sensor) {
            // [X,Y]
            //front sensors
            case "U8SENSOR":
                displacement = [5.11811, 1.77165];
                break;
            case "U9SENSOR":
                displacement = [0.0,0.0];
                break;
            case "U10SENSOR":
                displacement = [-5.11811, 1.77165];
                break;
            case "U11SENSOR":
                displacement = [4.527559, 1.77165];
                break;
            case "U12SENSOR":
                displacement = [0.0,0.0];
                break;
            case "U13SENSOR":
                displacement = [-5.11811, 1.77165];
                break;
                //dead
            case "U14SENSOR":
                displacement = [0,0];
                break;

                //back sensors
            case "U1SENSOR":
                displacement = [-6.003937,-3.54331];
                break;
            case "U2SENSOR":
                displacement = [6.003937,-3.54331];
                break;
            case "U3SENSOR":
                displacement = [-3.93701,0.393701];
                break;
            case "U4SENSOR":
                displacement = [3.93701, 0.393701];
                break;
            case "U5SENSOR":
                displacement = [-5.90551, -3.54331];
                break;
            case "U6SENSOR":
                displacement = [5.90551, -3.54331];
                break;
                //dead
            case "U7SENSOR":
                displacement = [0,0];
                break;
        }

        return displacement;
    }
}

function drawBoard() {
    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0.5 + x + 0, 0);
        mctx.lineTo(0.5 + x + 0, 300 + 0);
    }

    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0, 0.5 + x + 0);
        mctx.lineTo(300 + 0, 0.5 + x + 0);
    }

    mctx.strokeStyle = "black";
    mctx.stroke();

}