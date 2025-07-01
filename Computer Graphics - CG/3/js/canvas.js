var camera, scene, renderer;
var pauseCam = false, frontCam = false, perspCam = false, cam_change = false;
var meshes = [], index = 0;
var clock;

var windowSize = 100;
var angle0, angle1, angle2;

var geometry, material, mesh, object, texture, woodTexture, pauseTexture;
var object1, object2, object3;

var changeBaseLight = false, baseLight = true, dlight;
var changeLight1 = false, spotlight1 = true, slight1;
var changeLight2 = false, spotlight2 = true, slight2;
var changeLight3 = false, spotlight3 = true, slight3;
var changeLights = false, lighting = true;
var changeShading = false, lambert = true;  

var rotate1Neg = false, rotate1Pos = false, rotate2Neg = false, rotate2Pos = false, rotate3Neg = false, rotate3Pos = false, rotate = false;
var paused = false, changePaused = false, reset = false;

var move_speed = 2;

function createScene() {
    'use strict';

    clock = new THREE.Clock(true);

    var textureLoader = new THREE.TextureLoader();
    texture = textureLoader.load('/textures/origami.png');
    woodTexture = textureLoader.load('/textures/wood.png');
    pauseTexture = textureLoader.load('/textures/pause.png');  

    scene = new THREE.Scene();
    
    object1 =  new THREE.Object3D();
    object2 =  new THREE.Object3D();
    object3 =  new THREE.Object3D();

    createPolyhedron(140, 0.2, 90, 0, -3.6 , -50, false); //plane floor
    createPolyhedron(100, 7.5, 14, 0, 0, -50, true); //palanque
    createPolyhedron(50, 5, 4, 0, -1, -41, true); //bigger step
    createPolyhedron(50, 2.5, 4, 0, -2.25, -38, true); //smaller step

    createPauseScreen();
    createOrigami1();
    createOrigami2();
    createOrigami3();

    createLamps();

}

function createLamps(){
    createCone(4, 10, 0x5b8899, -35, 45, -47);
    createCone(4, 10, 0x5b8899, 0, 45, -47);
    createCone(4, 10, 0x5b8899, 35, 45, -47);
    createBall(2, 0, 0xdfe9ec, -35, 40, -47);
    createBall(2, 0, 0xdfe9ec, 0, 40, -47);
    createBall(2, 0, 0xdfe9ec, 35, 40, -47);
}

function createPauseScreen(){
    geometry = new THREE.PlaneGeometry( 50, 20 );
    material = new THREE.MeshBasicMaterial( {map: pauseTexture} );
    plane = new THREE.Mesh( geometry, material );
    plane.position.set(0, 0, 20);
    scene.add( plane );

    geometry = new THREE.PlaneGeometry( 55, 25 );
    material = new THREE.MeshBasicMaterial( {color:0x0000ba} );
    plane = new THREE.Mesh( geometry, material );
    plane.position.set(0, 0, 19);
    scene.add( plane );
    
}

function createOrigami1(){
    const geometry = new THREE.BufferGeometry();

    const vertices = new Float32Array( [
        
        0.0, 33.5,  -0.5,
        -13.25, 20.25,  0.5,
        0.0,  7.0,  -0.5,
        
        0.0,  7.0,  -0.5,
        13.25, 20.25,  0.5,
        0.0, 33.5,  -0.5,

    ] );

    const uvs = new Float32Array([
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,

        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0
    ]);

    // itemSize = 3 because there are 3 values (components) per vertex
    geometry.setAttribute( "position", new THREE.BufferAttribute( vertices, 3 ) );
    geometry.setAttribute( "uv", new THREE.BufferAttribute( uvs, 2 ) );
    geometry.computeVertexNormals();
    const material = new THREE.MeshLambertMaterial( {map: texture, side:THREE.DoubleSide} );
    const mesh = new THREE.Mesh( geometry, material );
    meshes[index++] =  mesh;

    object1.add(mesh);
    object1.position.set(-35,0,-50);
    scene.add(object1);

    var mesh2 = mesh.clone();
    mesh2.material = new THREE.MeshPhongMaterial({map:texture, side:THREE.DoubleSide});
    meshes[index++] =  mesh2;

    var mesh3 = mesh.clone();
    mesh3.material = new THREE.MeshBasicMaterial({map:texture, side:THREE.DoubleSide});
    meshes[index++] =  mesh3;
    
}

function createOrigami2(){
    const geometry = new THREE.BufferGeometry();

    const vertices = new Float32Array( [
        
       //left side
       0.0, 27.5, -0.5, 
       -5.0, 25.5, 0.5, 
       0.0, 7.0, -0.5, 

       -6.0, 28.5, 0.5,
       0.0, 27.5, -0.5, 
       -5.0, 25.5, 0.5, 

       0.0, 27.5, -0.5, 
       0.0, 33.5, -0.5, 
       -6.0, 28.5, 0.5,

       //right side
       0.0, 27.5, -0.5, 
       5.0, 25.5, 0.5, 
       0.0, 7.0, -0.5, 

       6.0, 28.5, 0.5,
       0.0, 27.5, -0.5, 
       5.0, 25.5, 0.5, 

       0.0, 27.5, -0.5, 
       0.0, 33.5, -0.5, 
       6.0, 28.5, 0.5

    ] );

    const uvs = new Float32Array([
        0.4108, 1.0, 
        0.1946, 1.0, 
        0.0, 0.0, 

        //white
        0.0, 0.0, 
        0.0, 0.0, 
        0.0, 0.0, 

        0.7622, 0.7622, 
        1.0, 1.0, 
        0.6649, 1.0, 

        1.0, 0.4108, 
        1.0, 0.1946,
        0.0, 0.0,

        //white
        0.0, 0.0, 
        0.0, 0.0,  
        0.0, 0.0, 

        0.7622, 0.7622, 
        1.0, 1.0, 
        1.0, 0.6649 
    ]);

    geometry.setAttribute( 'position', new THREE.BufferAttribute( vertices, 3 ) );
    geometry.setAttribute( 'uv', new THREE.BufferAttribute( uvs, 2 ) );
    geometry.computeVertexNormals();
    const material = new THREE.MeshLambertMaterial( {map:texture, side:THREE.DoubleSide} );
    const mesh = new THREE.Mesh( geometry, material );
    meshes[index++] =  mesh;
    object2.add(mesh);
    object2.position.set(0, 0, -50);
    scene.add(object2);

    var mesh2 = mesh.clone();
    mesh2.material = new THREE.MeshPhongMaterial({ map:texture, side:THREE.DoubleSide });
    meshes[index++] =  mesh2;

    var mesh3 = mesh.clone();
    mesh3.material = new THREE.MeshBasicMaterial({ map:texture, side:THREE.DoubleSide});
    meshes[index++] =  mesh3;
}

function createOrigami3(){
    const geometry = new THREE.BufferGeometry();

    const vertices = new Float32Array( [
        
        //body
        -5.9, 10.0, -1.0,
        -2.4, 5.5,  -0.5,
        0.1, 9.7,  -0.5,
   
        0.1, 9.7,  -0.5,
        -2.4, 5.5,  -0.5,
        1.6, 6.0,  0.0,
        
        0.1, 9.7,  -0.5,
        1.6, 6.0,  0.0,
        5.5, 9.5,  0.0,
        
        1.6, 6.0,  0.0, 
        5.5, 6.5,  0.0,
        5.5, 9.5,  0.0,

        //neck
        5.5, 18.5, 0.0,
        5.5, 6.5,  0.0,
        8.0, 9.2,  0.5,

        5.5, 18.5, 0.0, 
        8.0, 9.2,  0.5, 
        6.5, 18.0, 0.5,

        //head
        6.5, 19.0, 0.5,
        10.0, 16.2, 2.2,    
        5.5, 18.5, 0.0
        
    ] );

    const uvs = new Float32Array([
        1.0, 1.0,
        0.7622, 0.7622,
        0.6649, 1.0,

        0.0, 0.0,
        0.0, 0.0,
        0.0, 0.0,

        0.4108, 1.0,
        0.1946, 1.0,
        0.3081, 0.7622,
        
        0.1946, 1.0,
        0.1514, 0.8108,
        0.3081, 0.7622,
        
        0.0324, 0.1946,
        0.1514, 0.8108,
        0.0, 0.6973,

        0.0324, 0.1946,
        0.0, 0.6973,
        0.0, 0.2162,

        0.0595, 0.1514,
        1.0, 1.0,    
        0.0324, 0.1946

    ]);

    geometry.setAttribute( 'position', new THREE.BufferAttribute( vertices, 3 ) );
    geometry.setAttribute( "uv", new THREE.BufferAttribute( uvs, 2 ) );
    geometry.computeVertexNormals();
    const material = new THREE.MeshLambertMaterial( {side:THREE.DoubleSide, map:texture } );
    const mesh = new THREE.Mesh( geometry, material );
    meshes[index++] =  mesh;
    object3.add(mesh);
    object3.position.set(35, 8, -50);
    scene.add(object3);

    var mesh2 = mesh.clone();
    mesh2.material = new THREE.MeshPhongMaterial({side:THREE.DoubleSide, map:texture });
    meshes[index++] =  mesh2;

    var mesh3 = mesh.clone();
    mesh3.material = new THREE.MeshBasicMaterial({map:texture, side:THREE.DoubleSide });
    meshes[index++] =  mesh3;

}

function createPolyhedron(xSize, ySize, zSize, x, y, z, wood) {
    object = new THREE.Object3D();

    geometry = new THREE.BoxGeometry( xSize, ySize, zSize );
    if (wood)
        material = new THREE.MeshPhongMaterial( { map:woodTexture} );
    else      
        material = new THREE.MeshPhongMaterial( { color:0x913831});

    mesh = new THREE.Mesh( geometry, material );
    object.add(mesh);
    object.position.set(x, y, z);

    scene.add(object);
}

function createCone(radius, height, color, x, y, z) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color});

    geometry = new THREE.ConeGeometry(radius, height, 24);
    mesh = new THREE.Mesh(geometry, material);
    mesh.rotation.y = Math.PI / 4;
    
    object.add(mesh);
    object.position.set(x, y, z);
    scene.add(object);
}

function createBall(radius, thetaStart, color, x, y, z) {
    'use strict';
    object = new THREE.Object3D();
    
    material = new THREE.MeshBasicMaterial({ color: color }); 

    geometry = new THREE.SphereGeometry(radius, 10, 10, 0, Math.PI * 2, thetaStart);
    mesh = new THREE.Mesh(geometry, material);
    
    object.add(mesh);
    object.position.set(x, y, z);
    scene.add(object);
}

function changeCamera() {
    'use strict';
    
    if (pauseCam)
    {
        var aspectRatio = window.innerWidth / window.innerHeight;
        camera = new THREE.OrthographicCamera(-aspectRatio * windowSize /  2,
        aspectRatio * windowSize / 2,
        windowSize / 2,
        -windowSize /  2,
        1,
        100); 
        camera.position.x = 0;
        camera.position.y = 0;
        camera.position.z = 25;
        camera.lookAt(scene.position);
    }

    else if (frontCam)
    {
        var aspectRatio = window.innerWidth / window.innerHeight;
        camera = new THREE.OrthographicCamera(-aspectRatio * windowSize /  2, aspectRatio * windowSize / 2, windowSize / 2, -windowSize /  2, 1, 100 );
        camera.position.x = 0;
        camera.position.y = 0;
        camera.position.z = 10;
        camera.lookAt(scene.position);

    }
    else if (perspCam)
    {
        camera = new THREE.PerspectiveCamera(70,
        window.innerWidth / window.innerHeight,
        1,
        1000);
        camera.position.x = 50;
        camera.position.y = 40;
        camera.position.z = 10;
        camera.lookAt(object2.position);
    }
    cam_change = false;
}

function createLights(){
    dlight = new THREE.DirectionalLight(0xffffff, 0.5);
    dlight.position.set(0, 30, -20);
    dlight.target.position.set(0, 0, -50);
    scene.add(dlight);
    scene.add(dlight.target);
    
    slight1 = new THREE.SpotLight(0xffffff, 2, 80, Math.PI/6, 0.5, 0.75);
    slight1.position.set(-35, 39, -47);
    slight1.target.position.set(-35, 0, -53.5);
    scene.add(slight1);
    scene.add(slight1.target);

    slight2 = new THREE.SpotLight(0xffffff, 2, 80, Math.PI/6, 0.5, 0.75);
    slight2.position.set(0, 39, -47)
    slight2.target.position.set(0, 7, -50);
    scene.add(slight2);
    scene.add(slight2.target);

    slight3 = new THREE.SpotLight(0xffffff, 2, 80, Math.PI/6, 0.5, 0.75);
    slight3.position.set(35, 39, -47)
    slight3.target.position.set(35, 7, -50);
    scene.add(slight3);
    scene.add(slight3.target);

}

function onKeyDownPaused(e) {
    'use strict';

    if (e.keyCode == 65){ //A
        changeShading = true;  
    }
    if (e.keyCode == 83){ //S
        changeLights = true;
    }
    if (e.keyCode == 68){ //D
        changeBaseLight = true;
    }
    if (e.keyCode == 90){ //Z
        changeLight1 = true;
    }
    if (e.keyCode == 88){ //X
        changeLight2 = true;
    }
    if (e.keyCode == 67){ //C
        changeLight3 = true;
    }
    if (e.keyCode == 32){ //Space
        changePaused = true;
    }
    if (e.keyCode == 99 || e.keyCode == 51){ //3
        reset = true;
    }
}

function onKeyDownUnpaused(e) {
    'use strict';
    
    if (e.keyCode == 49 || e.keyCode == 97){ //1
        frontCam = false;
        perspCam = true;
        cam_change = true;
    }
    if (e.keyCode == 98 || e.keyCode == 50){ //2
        frontCam = true;
        perspCam = false;
        cam_change = true;
    }
    if (e.keyCode == 81){ //Q
        rotate1Neg = true;  
        rotate = true;
    }
    if (e.keyCode == 87){ //W
        rotate1Pos = true;  
        rotate = true;
    }
    if (e.keyCode == 69){ //E
        rotate2Neg = true;  
        rotate = true;
    }
    if (e.keyCode == 82){ //R
        rotate2Pos = true;  
        rotate = true;
    }
    if (e.keyCode == 84){ //T
        rotate3Neg = true;  
        rotate = true;
    }
    if (e.keyCode == 89){ //Y
        rotate3Pos = true;  
        rotate = true;
    }
    if (e.keyCode == 65){ //A
        changeShading = true;  
    }
    if (e.keyCode == 83){ //S
        changeLights = true;
    }
    if (e.keyCode == 68){ //D
        changeBaseLight = true;
    }
    if (e.keyCode == 90){ //Z
        changeLight1 = true;
    }
    if (e.keyCode == 88){ //X
        changeLight2 = true;
    }
    if (e.keyCode == 67){ //C
        changeLight3 = true;
    }
    if (e.keyCode == 32){ //Space
        changePaused = true;
    }
    if (e.keyCode == 99 || e.keyCode == 51){ //3
        reset = true;
    }
    clock.start();
}

function onKeyUp(e) {
    'use strict';
    
    if (e.keyCode == 81){ //Q
        rotate1Neg = false;  
    }
    if (e.keyCode == 87){ //W
        rotate1Pos = false;  
    }
    if (e.keyCode == 69){ //E
        rotate2Neg = false;  
    }
    if (e.keyCode == 82){ //R
        rotate2Pos = false;  
    }
    if (e.keyCode == 84){ //T
        rotate3Neg = false;  
    }
    if (e.keyCode == 89){ //Y
        rotate3Pos = false;  
    }

    if (rotate1Neg == false && rotate1Pos == false &&  rotate2Neg == false && rotate2Pos == false &&  rotate3Neg == false && rotate3Pos == false)
        rotate = false;

}

function onResize() {
    'use strict';

    renderer.setSize(window.innerWidth, window.innerHeight);
    if (frontCam || pauseCam) {
        if (window.innerHeight > 0 && window.innerWidth > 0) {
            
            camera.aspect = window.innerWidth / window.innerHeight;
            var aspectRatio = window.innerWidth / window.innerHeight;
            camera.left = -aspectRatio * windowSize /  2;
            camera.right = aspectRatio * windowSize / 2;
            camera.top = windowSize / 2;
            camera.bottom =-windowSize /  2;
            camera.lookAt(scene.position);
            camera.updateProjectionMatrix();

        }
    }

    else if (perspCam){
        if (window.innerHeight > 0 && window.innerWidth > 0) {
            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();
        }
    }
}

function initVR () {
    document.body.appendChild(VRButton.createButton(renderer));
}

function render() {
    'use strict';
    renderer.render(scene, camera);
}

function initRender() {
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    renderer.outputEncoding = THREE.sRGBEncoding;
    renderer.xr.enabled = true; 
    document.body.appendChild( renderer.domElement );
}

function init() {
    'use strict';
    initRender();

    createScene();
    perspCam = true;
    changeCamera();
    createLights();
    initVR();
    
    window.addEventListener("keydown", onKeyDownUnpaused);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);

}

function turnBaseLight(){
    baseLight = !baseLight;
    if (baseLight){
        scene.add(dlight);
        scene.add(dlight.target);
    }
        
    else{
        scene.remove(dlight);
        scene.remove(dlight.target);
    }
    changeBaseLight = false;
}

function turnSpotlight1(){
    spotlight1 = !spotlight1; 
    if (spotlight1){
        scene.add(slight1);
        scene.add(slight1.target)
    }
        
    else{
        scene.remove(slight1);
        scene.remove(slight1.target)
    }
    changeLight1 = false;
}

function turnSpotlight2(){
    spotlight2 = !spotlight2;
    if (spotlight2){
        scene.add(slight2);
        scene.add(slight2.target)
    }
        
    else{
        scene.remove(slight2);
        scene.remove(slight2.target)
    }
    changeLight2 = false;
}

function turnSpotlight3(){
    spotlight3 = !spotlight3;
    if (spotlight3){
        scene.add(slight3);
        scene.add(slight3.target)
    }
        
    else{
        scene.remove(slight3);
        scene.remove(slight3.target)
    }
    changeLight3 = false;
}

function rotateOrigamis(){

    seconds = clock.getDelta();

    if (rotate1Neg){
        object1.rotation.y -= move_speed * seconds;
    }
    
    if (rotate1Pos){
        object1.rotation.y += move_speed * seconds;
    }

    if (rotate2Neg){
        object2.rotation.y -= move_speed * seconds;
    }

    if (rotate2Pos){
        object2.rotation.y += move_speed * seconds;
    }
    
    if (rotate3Neg) {
        object3.rotation.y -= move_speed * seconds;
    }

    if (rotate3Pos) {
        object3.rotation.y += move_speed * seconds;
    }
}

function changeMaterials(){
    lambert = !lambert;
    if (lambert){
        object1.remove(meshes[1]);
        object1.add(meshes[0]);
        object2.remove(meshes[4]);
        object2.add(meshes[3]);
        object3.remove(meshes[7]);
        object3.add(meshes[6]);
    }

    else{
        object1.remove(meshes[0]);
        object1.add(meshes[1]);
        object2.remove(meshes[3]);
        object2.add(meshes[4]);
        object3.remove(meshes[6]);
        object3.add(meshes[7]);
    }
    changeShading = false;
}

function changeLighting(){
    lighting = !lighting;
    if (!lighting){
        if (lambert){
            object1.remove(meshes[0]);
            object1.add(meshes[2]);
            object2.remove(meshes[3]);
            object2.add(meshes[5]);
            object3.remove(meshes[6]);
            object3.add(meshes[8]);
        }

        else{
            object1.remove(meshes[1]);
            object1.add(meshes[2]);
            object2.remove(meshes[4]);
            object2.add(meshes[5]);
            object3.remove(meshes[7]);
            object3.add(meshes[8]);
        }
    }

    else{
        if (lambert){
            object1.remove(meshes[2]);
            object1.add(meshes[0]);
            object2.remove(meshes[5]);
            object2.add(meshes[3]);
            object3.remove(meshes[8]);
            object3.add(meshes[6]);
        }
        else{
            object1.remove(meshes[2]);
            object1.add(meshes[1]);
            object2.remove(meshes[5]);
            object2.add(meshes[4]);
            object3.remove(meshes[8]);
            object3.add(meshes[7]);
        }
        
    }
    changeLights = false;
    changeShading = false;
}


function update() {
    if (cam_change)
        changeCamera();
        
    if (changeBaseLight)
        turnBaseLight();

    if (changeLight1)
        turnSpotlight1();

    if (changeLight2)
        turnSpotlight2();

    if (changeLight3)
        turnSpotlight3();
        
    if (rotate) 
        rotateOrigamis();
    
    if (changeShading && lighting)
        changeMaterials();
    
    if (changeLights)
        changeLighting();
    
    clock.start();
}

function display() {
    render();
}

function resetBooleans(){
    frontCam = false, perspCam = true;
    changeBaseLight = false, baseLight = true;
    changeLight1 = false, spotlight1 = true;
    changeLight2 = false, spotlight2 = true;
    changeLight3 = false, spotlight3 = true;
    changeLights = false, lighting = true;
    changeShading = false, lambert = true;
}

function animate() {
    'use strict';

    if (paused && reset){
        resetBooleans();
        meshes = [];
        index = 0;
        createScene();
        createLights();
        changeCamera();
        changePaused = true;
        reset = false;
    }  

    if (changePaused){
        paused = !paused;
        changePaused = false;
        
        if (paused)
        {
            pauseCam = true;
            clock.stop();
            window.removeEventListener("keydown", onKeyDownUnpaused);
            window.addEventListener("keydown", onKeyDownPaused);
        }
        
        if (!paused)
        {
            pauseCam = false;
            window.removeEventListener("keydown", onKeyDownPaused);
            window.addEventListener("keydown", onKeyDownUnpaused);
        }
        cam_change = true;
    }

    update();
    display();
    
    requestAnimationFrame(animate);
    renderer.setAnimationLoop(render);
}

