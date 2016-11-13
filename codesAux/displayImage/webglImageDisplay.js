var glObj={}

function render(image, idCanvas) {    
    // Get A WebGL context

    var canvas = document.getElementById(idCanvas);;
    var gl=null;
    if(glObj[idCanvas]===undefined){
        gl = getWebGLContext(canvas);
        if (!gl) {
            return;
        }
    // setup GLSL program
        vertexShader = createShaderFromScriptElement(gl, "2d-vertex-shader");
        fragmentShader = createShaderFromScriptElement(gl, "2d-fragment-shader");
        program = createProgram(gl, [vertexShader, fragmentShader]);
        gl.useProgram(program);
    }
    else{
        gl=glObj[idCanvas].gl;
    }
    // look up where the vertex data needs to go.
    var positionLocation;
    if(glObj[idCanvas]===undefined)
        positionLocation = gl.getAttribLocation(program, "a_position");
    else
        positionLocation= glObj[idCanvas].positionLocation;
    
    // provide texture coordinates for the rectangle.
    if(glObj[idCanvas]===undefined){
        var texCoordLocation = gl.getAttribLocation(program, "a_texCoord");
        var texCoordBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
        
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
            0.0,  0.0,
            1.0,  0.0,
            0.0,  1.0,
            0.0,  1.0,
            1.0,  0.0,
            1.0,  1.0]), gl.STATIC_DRAW);
        gl.enableVertexAttribArray(texCoordLocation);
        gl.vertexAttribPointer(texCoordLocation, 2, gl.FLOAT, false, 0, 0);
    }
    // Create a texture.
    //gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    var texture =null
    
    if(glObj[idCanvas]!==undefined){
        texture=glObj[idCanvas].texture;
        
    }
    else{
        texture=gl.createTexture();
        // Set the parameters so we can render any size image.
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER,  gl.LINEAR_MIPMAP_LINEAR);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
        //gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);

    }
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    
    // Upload the image into the texture.
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    //var imgObj= new Image()
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA,gl.RGBA, gl.UNSIGNED_BYTE, image);
    //imgObj.src='data:image/png;base64,'+image;
    gl.generateMipmap(gl.TEXTURE_2D);
    if(glObj[idCanvas]===undefined){
    
    
        // lookup uniforms
        var resolutionLocation = gl.getUniformLocation(program, "u_resolution");
        
        // set the resolution
        gl.uniform2f(resolutionLocation, canvas.width, canvas.height);        
    }
    else{
      
    
    }
       // Create a buffer for the position of the rectangle corners.
    var buffer=null;
    if(glObj[idCanvas]!==undefined){
        buffer=glObj[idCanvas].buffer;

    }
    else{
        buffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    

    }
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);
        
    // Set a rectangle the same size as the image.
    setRectangle(gl, 0, 0, canvas.width, canvas.height);
        
    // Draw the rectangle.
    gl.drawArrays(gl.TRIANGLES, 0, 6);
    gl.bindTexture(gl.TEXTURE_2D, null);
    if(glObj[idCanvas]===undefined){
        glObj[idCanvas]={
            texture:texture,
            gl:gl,
            buffer:buffer,
            image:image,
            positionLocation:positionLocation
        };
    }
}

function randomInt(range) {
  return Math.floor(Math.random() * range);
}

function setRectangle(gl, x, y, width, height) {
  var x1 = x;
  var x2 = x + width;
  var y1 = y;
  var y2 = y + height;
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
     x1, y1,
     x2, y1,
     x1, y2,
     x1, y2,
     x2, y1,
     x2, y2]), gl.STATIC_DRAW);
}
