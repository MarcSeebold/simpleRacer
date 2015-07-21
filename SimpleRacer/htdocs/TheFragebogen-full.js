//ac3b81a20384e903a99550955d57d12f21879df5 Tue, 16 Jun 2015 16:15:47 +0200 Linux marc-GX623-PX603 3.19.0-20-generic #20-Ubuntu SMP Fri May 29 10:10:47 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux
/**
 During the course of the questionnaire, messages are generated to provide
 some information to the questionnaire applicator or even to the user.<br>
 The messages are produced along all the questionnaire in different locations
 of the code.<br>
 The messages are also subdivided in five types according to their relevance:<br>
 1. Fatal<br>
 2. Error<br>
 3. Warn<br>
 4. Info<br>
 5. Debug<br>
 All the messages are saved in an array and logged into the console together
 with the message type and location on the code.
 @class LogConsole
 */
function LogConsole() {
    this.logMessages = new Array();
    this.debug("LogConsole.constructor()", "Start");
}
/**
 Saves a new LogMessage of type debug in the logMessages array and logs the
 message into the console.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogConsole.prototype.debug = function (where, msg) {
    this.logMessages.push(new LogMessage("DEBUG", where, msg));
    if (console.debug === undefined) //For IE console.debug is not defined.
        console.debug = console.log;
    console.debug("DEBUG: " + where + ": " + msg);
};
/**
 Saves a new LogMessage of type info in the logMessages array and logs the
 message into the console.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogConsole.prototype.info = function (where, msg) {
    this.logMessages.push(new LogMessage("INFO", where, msg));
    console.info("INFO: " + where + ": " + msg);
};
/**
 Saves a new LogMessage of type warn in the logMessages array and logs the
 message into the console. The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogConsole.prototype.warn = function (where, msg) {
    this.logMessages.push(new LogMessage("WARN", where, msg));
    console.warn("WARN: " + where + ": " + msg);
};
/**
 Saves a new LogMessage of type error in the logMessages array and logs the
 message into the console.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogConsole.prototype.error = function (where, msg) {
    this.logMessages.push(new LogMessage("ERROR", where, msg));
    console.error("ERROR: " + where + ": " + msg);
};
/**
 Saves a new LogMessage of type fatal in the logMessages array and logs the
 message into the console.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogConsole.prototype.fatal = function (where, msg) {
    this.logMessages.push(new LogMessage("FATAL", where, msg));
    console.error("FATAL: " + where + ": " + msg);
};

/**
 During the course of the questionnaire, messages are generated to provide
 some information to the questionnaire applicator or even to the user.<br>
 The messages are produced along all the questionnaire in different locations
 of the code.<br>
 The messages are also subdivided in five types according to their relevance:<br>
 1. Fatal<br>
 2. Error<br>
 3. Warn<br>
 4. Info<br>
 5. Debug<br>
 All the messages are saved in an array and logged into the console together
 with the message type and location on the code.<br>
 A new log window is created where the messages already saved in the array and
 the new messages will be all logged into it again.<br>
 The log window can be displayed or hidden by pressing simultaneously control
 and down arrow on the keyboard.
 @class LogWindow
 @augments LogConsole
 @param {array} logMessages contains the messages that were saved by the
 LogConsole and have already been logged into console
 */
function LogWindow(logMessages) {
    this.logMessages = logMessages || new Array();

    this.node = document.createElement("div");
    this.node.setAttribute("style", "visibility: hidden; overflow: auto; height: 15em; width: 30em; background-color: white; border-style: solid; border-width: 2px; position: absolute; bottom: 0; right: 0; text-align: center");
    var table = document.createElement("table");
    table.setAttribute("style", "border-collapse: collapse");
    this.node.appendChild(table);
    logMessages.length !== 0 ? this.updateWindow() : null;
    document.body.appendChild(this.node);

    document.addEventListener('keyup', (this.docKeyUp).bind(this));
    this.info("LogWindow.constructor()", "Start");
}
LogWindow.prototype = Object.create(LogConsole.prototype);
LogWindow.prototype.constructor = LogWindow;
/**
 Saves a new LogMessage of type debug in the logMessages array and logs the
 message into the console and into the log window.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogWindow.prototype.debug = function (where, msg) {
    this.logMessages.push(new LogMessage("DEBUG", where, msg));
    if (console.debug === undefined) //For IE console.debug is not defined.
        console.debug = console.log;
    console.debug("DEBUG: " + where + ": " + msg);
    this.insertRow(["DEBUG", where, msg]);
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Saves a new LogMessage of type info in the logMessages array and logs the
 message into the console and into the log window.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogWindow.prototype.info = function (where, msg) {
    this.logMessages.push(new LogMessage("INFO", where, msg));
    console.info("INFO: " + where + ": " + msg);
    this.insertRow(["INFO", where, msg]);
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Saves a new LogMessage of type warn in the logMessages array and logs the
 message into the console and into the log window.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogWindow.prototype.warn = function (where, msg) {
    this.logMessages.push(new LogMessage("WARN", where, msg));
    console.warn("WARN: " + where + ": " + msg);
    this.insertRow(["WARN", where, msg]);
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Saves a new LogMessage of type error in the logMessages array and logs the
 message into the console and into the log window.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogWindow.prototype.error = function (where, msg) {
    this.logMessages.push(new LogMessage("ERROR", where, msg));
    console.error("ERROR: " + where + ": " + msg);
    this.insertRow(["ERROR", where, msg]);
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Saves a new LogMessage of type fatal in the logMessages array and logs the
 message into the console and into the log window.<br>
 The messages are accompanied by the type and the location.
 @param {string} where location of message in the code
 @param {string} msg the message itself
 */
LogWindow.prototype.fatal = function (where, msg) {
    this.logMessages.push(new LogMessage("FATAL", where, msg));
    console.error("FATAL: " + where + ": " + msg);
    this.insertRow(["FATAL", where, msg]);
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Updates the log window by logging all the messages previously saved in the
 logMessages array into it.
 */
LogWindow.prototype.updateWindow = function () {
    for (var i in this.logMessages) {
        var array = new Array();
        for (var j in this.logMessages[i])
            array.push(this.logMessages[i][j]);
        this.insertRow(array);
    }
    this.node.scrollTop = this.node.scrollHeight;
};
/**
 Inserts a new row (message) into the log window.<br>
 The messages have colors according to their relevance type:<br>
 1. Fatal - Red<br>
 2. Error - Red<br>
 3. Warn - Yellow<br>
 4. Info - Blue<br>
 5. Debug - Green<br>
 */
LogWindow.prototype.insertRow = function () {
    var table = this.node.getElementsByTagName("table")[0];
    table.insertRow();
    var row = table.rows[table.rows.length - 1];
    row.setAttribute("style", "border-bottom:1pt solid black; text-align:center");
    switch (arguments[0][0]) {
        case "DEBUG":
            row.style.color = "green";
            break;

        case "INFO":
            row.style.color = "blue";
            break;

        case "WARN":
            row.style.color = "yellow";
            break;

        case "ERROR":
            row.style.color = "red";
            break;

        case "FATAL":
            row.style.color = "red";
            break;

        default:
            row.style.color = "black";
            break;
    }
    for (var i in arguments[0]) {
        row.insertCell();
        row.cells[row.cells.length - 1].innerHTML = arguments[0][i];
    }
};
/**
 Displays or hides the log window when the keys control and down arrow are
 pressed simultaneously on the keyboard.<br>
 */
LogWindow.prototype.docKeyUp = function (e) {
    if (e.ctrlKey && e.keyCode === 40) {
        var visibility = TheFragebogen.logger.node.style.visibility;
        TheFragebogen.logger.node.style.visibility = (visibility === "visible" ? "hidden" : "visible");
    }
};

/**
 During the course of the questionnaire, messages are generated to provide
 some information to the questionnaire applicator or even to the user.<br>
 The messages are produced along all the questionnaire in different locations
 of the code.<br>
 The messages are also subdivided in five types according to their relevance:<br>
 1. Fatal<br>
 2. Error<br>
 3. Warn<br>
 4. Info<br>
 5. Debug<br>
 @class LogMessage
 @param {string} logLevel type of message
 @param {string} where location
 @param {string} msg the message itself
 */
function LogMessage(logLevel, where, msg) {
    this.logLevel = (typeof logLevel === "string" ? logLevel : "DEBUG");
    this.where = (typeof where === "string" ? where : "");
    this.msg = msg;
}

/**
 The log-object that does all the logging.<br>
 Logs the first messages as a LogConsole. Upgraded later to a LogWindow.
 */
TheFragebogen = {
    logger: new LogConsole()
};
/**
 This is the first class of all the elements that can be used in a questionnaire.<br>
 The media elements, the questionnaire items with predefined answers or not
 and even the elements without displayable user interface belong to this class.<br>
 Most of the methods of this class are abstract and might be overridden.
 @abstract
 @class UIElement
 */
function UIElement() {
    this.uiCreated = false;
    this.enabled = false;
}
/**
 Checks if the UI is created.
 @returns {boolean} true if the UI is created, false if not
 */
UIElement.prototype.isUIcreated = function () {
    return this.uiCreated;
};
/**
 Creates the UI of the element.
 @abstract
 */
UIElement.prototype.createUI = function () {
    TheFragebogen.logger.debug("UIElement.createUI()", "This method must be overridden.");
};
/**
 Removes all references from and to elements created by createUI().
 @abstract
 */
UIElement.prototype.releaseUI = function () {
    TheFragebogen.logger.debug("UIElement.releaseUI()", "This method might need to be overridden.");
};
/**
 Gets relavant data stored in the element.
 @abstract
 */
UIElement.prototype.getData = function () {
    TheFragebogen.logger.debug("UIElement.getData()", "This method might need to be overridden.");
};
/**
 Compares the data stored in the element and the provided input data.
 @abstract
 */
UIElement.prototype._checkData = function () {
    TheFragebogen.logger.debug("UIElement._checkData()", "This method might need to be overridden.");
};
/**
 Sets the provided input data.
 @abstract
 */
UIElement.prototype.setData = function () {
    TheFragebogen.logger.debug("UIElement.setData()", "This method might need to be overridden.");
};
/**
 Checks if the UIElement is enabled.
 @abstract
 */
UIElement.prototype.isEnabled = function () {
    TheFragebogen.logger.debug("UIElement.setEnabled()", "This method might need to be overridden.");
    return this.enabled;
};
/**
 Sets the UIElement enabled.
 @abstract
 */
UIElement.prototype.setEnabled = function () {
    TheFragebogen.logger.debug("UIElement.setEnabled()", "This method might need to be overridden.");
};
/**
 Converts the element into a string.
 @abstract
 */
UIElement.prototype.toString = function () {
    TheFragebogen.logger.debug("UIElement.toString()", "This method might need to be overridden.");
};
/**
 Returns the class of the element.
 @returns {string} class of the element
 */
UIElement.prototype.getType = function () {
    return this.constructor.name;
};
/**
 Checks if the element is ready.
 @abstract
 @returns {boolean} true if the element is ready, false if not
 */
UIElement.prototype.isReady = function () {
    TheFragebogen.logger.debug("UIElement.isReady()", "This method might need to be overridden.");
    return true;
};


/**
 This is a class of all elements that somehow possess an interactive process.<br>
 The media elements, the questionnaire items with predefined answers or not and
 even the elements without displayable user interface elements belong to this
 class.<br>
 It is by default: enabled = false.
 @abstract
 @class UIElementInteractive
 @augments UIElement
 */
function UIElementInteractive() {
    UIElement.call(this);
    this.enabled = false;
    this.onReadyCallback = null;
}
UIElementInteractive.prototype = Object.create(UIElement.prototype);
UIElementInteractive.prototype.constructor = UIElementInteractive;
/**
 Checks if the UIElement is enabled.
 @returns {boolean} true if the element is enabled, false if not
 */
UIElementInteractive.prototype.isEnabled = function () {
    return this.enabled;
};
/**
 Sets the UIElement enabled.
 @abstract
 @param {boolean} enabled enables the element
 */
UIElementInteractive.prototype.setEnabled = function (enabled) {
    this.enabled = enabled;
    TheFragebogen.logger.debug("UIElementInteractive.setEnabled()", "This method might need to be overridden.");
};
/**
 Set a callback that informs, if this element is ready.
 @param {function} onReadyCallback
*/
UIElementInteractive.prototype.setOnReadyCallback = function (onReadyCallback) {
    if (typeof onReadyCallback === "function") {
        TheFragebogen.logger.debug("UIElementInteractive.setOnReadyCallback()", "called");
        this.onReadyCallback = onReadyCallback;
    }
};
/**

*/
UIElementInteractive.prototype._sendReadyCallback = function () {
    if (typeof this.onReadyCallback === "function") {
        TheFragebogen.logger.debug("UIElementInteractive._sendReadyCallback()", "called");
        this.onReadyCallback();
    }
};
/**
 Marks a required element which is without answer, after trying to go the next
 screen.<br>
 The next button will just be enabled after answering all required elements.
 @abstract
 */
UIElementInteractive.prototype.markRequired = function () {
    TheFragebogen.logger.debug("UIElementInteractive.markRequired()", "This method should be overridden.");
};

/**
 A non-interactive UI element that shows HTML.<br>
 Should not contain any interactive content.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>HTML from the parameters here</div>
 @class UIElementHTML
 @augments UIElement
 @param {string} [className] CSS-style
 @param {string} html HTML
 */
function UIElementHTML(className, html) {
    UIElement.call(this);

    this.className = (typeof className === "string" ? className : "");
    this.html = (typeof html === "string" ? html : "");

    TheFragebogen.logger.debug("UIElementHTML()", "Set: className as " + this.className + " and html as " + this.html);
    if (typeof this.html != "string")
        TheFragebogen.logger.debug("UIElementHTML()", "Set: Html might need to be a string");

}
UIElementHTML.prototype = Object.create(UIElement.prototype);
UIElementHTML.prototype.constructor = UIElementHTML;
/**
 Creates the UI of the element.
 @returns {element} UI of the element.
 */
UIElementHTML.prototype.createUI = function () {
    this.node = document.createElement("div");
    this.node.className = this.className;
    this.node.innerHTML = this.html;
    return this.node;
};
UIElementHTML.prototype.releaseUI = function () {
};
/**
 Returns the html data stored in the element.
 @returns {array} html data stored in the index 0 of the array
 */
UIElementHTML.prototype.getData = function () {
    return [this.html];
};
/**
 Compares the html of the provided input data array and the html
 data stored in the element.
 @param {array} data html data stored in the index 0 of the array
 @returns {boolean} true if it is the same html data, false if not
 */
UIElementHTML.prototype._checkData = function (data) {
    return data[0] === this.html;
};
/**
 Calls the method _checkData() as no setData() is available for this type of element.
 @param {array} data html data stored in the index 0 of the array
 @returns {boolean} same as _checkData() returns: true if it is the same html data, false if not
 */
UIElementHTML.prototype.setData = function (data) {
    return this._checkData(data);
};
/**
 This is a class of all interactive elements that are questionnaire items, with
 predefined answers or not and also the elements without displayable user
 interface.<br> It is by default: enabled = false.<br>
 The elements of this class demand a question.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>Answer node</div>
 </div>
 @abstract
 @class QuestionnaireItem
 @augments UIElement
 @augments UIElementInteractive
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItem(className, question, required) {
    UIElementInteractive.call(this);

    this.node = null;

    this.className = (typeof className === "string" ? className : "");
    this.question = (typeof question === "string" ? question : "");
    this.required = (typeof required === "boolean" ? required : false);
    this.answer = null;

    TheFragebogen.logger.debug("QuestionnaireItem()", "Set: className as " + this.className + ", question as " + this.question + " and required as " + this.required);
}
QuestionnaireItem.prototype = Object.create(UIElementInteractive.prototype);
QuestionnaireItem.prototype.constructor = QuestionnaireItem;
/**
 Gets the question of the item.
 @returns {string} question
 */
QuestionnaireItem.prototype.getQuestion = function () {
    return this.question;
};
/**
 Sets the QuestionnaireItem enabled.
 @param {boolean} enable if the item must be enabled
 */
QuestionnaireItem.prototype.setEnabled = function(enable){
    this.enabled = this.isUIcreated() ? enable : false;
};
/**
 Checks if the item is answered.
 @returns {boolean} true if the item is answered, false if not
 */
QuestionnaireItem.prototype.isAnswered = function () {
    return this.answer != null;
};
/**
 Gets the answer of the item.
 @returns {string} answer
 */
QuestionnaireItem.prototype.getAnswer = function () {
    return this.answer;
};
/**
 Returns the list of answer options.
 @abstract
 @returns {string} 'undefined' by default
 */
QuestionnaireItem.prototype.getOptionList = function () {
    return "undefined";
};
/**
 Applies the answer to the UI.
 @abstract
 */
QuestionnaireItem.prototype._applyAnswerToUI = function () {
    TheFragebogen.logger.debug("QuestionnaireItem._applyAnswerToUI", "This method might need to be overridden.");
};
/**
 TODO change the description.
 Sets the answer.
 If the answer is allowed, the method this._sendReadyCallback() must be called.
 @abstract
 */
QuestionnaireItem.prototype.setAnswer = function () {
    this._sendReadyCallback();
    TheFragebogen.logger.debug("QuestionnaireItem.setAnswer()", "This method might need to be overridden.");
};
/**
 Checks if the element is ready.
 @returns {boolean} true if the element is ready, false if not
 */
QuestionnaireItem.prototype.isReady = function () {
    return this.isRequired() ? this.isAnswered() : true;
}
/**
 Checks if the element is required.
 @returns {boolean} true if the element is required, false if not
 */
QuestionnaireItem.prototype.isRequired = function () {
    return this.required;
};
/**
 Creates the UI by using the methods _createQuestionNode and _createAnswerNode.
 @returns {object} UI of the QuestionnaireItem.
 */
QuestionnaireItem.prototype.createUI = function () {
    if (this.isUIcreated()) return this.node;

    this.node = document.createElement("div");

    this.node.className = this.className;

    this.uiCreated = true;

    this.node.appendChild(this._createQuestionNode());
    this.node.appendChild(this._createAnswerNode());

    return this.node;
};
/**
 Removes all references from and to an element created by createUI().
 */
QuestionnaireItem.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;
};
/**
 Marks a required element which is without answer, after trying to go the next
 screen.<br>
 The next button will just be enabled after answering all required elements.
 */
QuestionnaireItem.prototype.markRequired = function () {
    if (this.node == null) return;

    if (!this.isReady()) this.node.className = "UIElementInteractiveRequired";
    else this.node.className = null;
};
/**
 Creates the node of the question
 @returns {object} node of the question
 */
QuestionnaireItem.prototype._createQuestionNode = function () {
    var node = document.createElement("div");
    node.innerHTML = this.question + (this.required ? "*" : "");
    return node;
};
/**
 Creates the node of the answer
 @abstract
 */
QuestionnaireItem.prototype._createAnswerNode = function () {
    TheFragebogen.logger.debug("QuestionnaireItem._createAnswerNode()", "This method might need to be overridden.");
};

/**
 This a class of items that are not available to the user and cannot be seen.<br>
 These elements can be used by the questionnaire constructor in order to<br>
 have control over the user and to get some specific informations.<br>
 It is by default: enabled = false.<br>
 @abstract
 @class QuestionnaireItemInvisible
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 */
function QuestionnaireItemInvisible() {
    QuestionnaireItem.apply(this, arguments);
}
QuestionnaireItemInvisible.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemInvisible.prototype.constructor = QuestionnaireItemInvisible;
/**
 This is an item that records the time spent on a screen, in milliseconds.<br>
 The answer of the item is the time between calling the functions createUI() and
 isReady() of the screen.<br>
 Every time isReady() is called the time is updated.<br>
 This item has a fixed question: "Screen Duration".<br>
 It is not displayable to the user.
 @class QuestionnaireItemScreenDuration
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemInvisible

 TODO Actually this is not a questionnaireItem in sense of inheritance, but it gives an answer.
 TODO This will not cope well with back navigation!
 */
function QuestionnaireItemScreenDuration() {
    QuestionnaireItemInvisible.call(this, null, "Screen Duration", false);
    this.startTime = null;
}
QuestionnaireItemScreenDuration.prototype = Object.create(QuestionnaireItemInvisible.prototype);
QuestionnaireItemScreenDuration.prototype.constructor = QuestionnaireItemScreenDuration;
/**
 Starts to count the duration of the screen.
 */
QuestionnaireItemScreenDuration.prototype.createUI = function () {
    this.startTime = new Date().getTime();
};
/**
 Stops counting and records the time spent on the screen in milliseconds.
 @returns {boolean} returns always true
 */
QuestionnaireItemScreenDuration.prototype.isReady = function () {
    this.answer = new Date().getTime() - this.startTime;
    return true;
};
/**
 Returns the question ("Screen Duration") and answer (time spent on screen) of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemScreenDuration.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
/**
 Compares the question ("Screen Duration") of the provided input data array and
 the question stored in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question, false if not
 */
QuestionnaireItemScreenDuration.prototype._checkData = function (data) {
    return (data[0] == this.question);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemScreenDuration.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};

/**
 Questionnaire item which returns the date and time when a screen was accessed.<br>
 The answer of the item is the time and date when the function createUI ()of the
 screen was called.<br>
 This item has a fixed question: "Screen Date and Time".<br>
 It is not displayable to the user.
 @class QuestionnaireItemScreenDateTime
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemInvisible

 TODO This will not cope well with back navigation!
 */
function QuestionnaireItemScreenDateTime() {
    QuestionnaireItemInvisible.call(this, null, "Screen Date and Time", false);
}
QuestionnaireItemScreenDateTime.prototype = Object.create(QuestionnaireItemInvisible.prototype);
QuestionnaireItemScreenDateTime.prototype.constructor = QuestionnaireItemScreenDateTime;
/**
 Records the date and time that the screen was created.
 */
QuestionnaireItemScreenDateTime.prototype.createUI = function () {
    this.answer = new Date().toString();
};
QuestionnaireItemScreenDateTime.prototype.isReady = function () {
    return Boolean(this.answer);
};
/**
 Returns the question ("Screen Date and Time") and answer (date and time when the screen was accessed) of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemScreenDateTime.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
/**
 Compares the question ("Screen Date and Time") the provided input data array
 and the question stored in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question, false if not
 */
QuestionnaireItemScreenDateTime.prototype._checkData = function (data) {
    return (data[0] == this.question);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemScreenDateTime.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};

/**
 This item is only used by the questionnaire constructor in order to record any
 kind of information that he might judge as useful for himself or for the
 questionnaire. It is hidden from the users.<br>
 The information which is recorded and hidden can be stored in the parameters
 question and answer.
 @class QuestionnaireItemHidden
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemInvisible
 @param {string} question first useful information
 @param {string} answer second useful information

 TODO Actually this is not a questionnaireItem in sense of inheritance, but it gives an answer.
 */
function QuestionnaireItemHidden(question, answer) {
    QuestionnaireItemInvisible.call(this, null, question, false);
    this.answer = answer;
}
QuestionnaireItemHidden.prototype = Object.create(QuestionnaireItemInvisible.prototype);
QuestionnaireItemHidden.prototype.constructor = QuestionnaireItemHidden;
/**
 @abstract
 */
QuestionnaireItemHidden.prototype.createUI = function () {
};
/**
 @abstract
 */
QuestionnaireItemHidden.prototype.releaseUI = function () {
};
/**
 Returns the question and answer of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemHidden.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
/**
 Compares the question and the answer of the provided input data array with the
 question and the answer stored in the element.
 @param {array} data question (index 0) and answer (index 1) stored in the array
 @returns {boolean} true if it is the same question and the same answer, false if not
 */
QuestionnaireItemHidden.prototype._checkData = function (data) {
    return (data[0] == this.question && data[1] == this.answer);
};
/**
 Sets the question and answer of the provided input data as the question and
 answer of the element.<br>
 The question and the answer of the provided input data must be the same as the
 question stored in the element.
 TODO: change this function, it doesn´t do anything.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemHidden.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.question = data[0];
    this.setAnswer(data[1]);
    return true;
};

/**
 @class QuestionnaireItemHiddenURL
 This item is only used by the questionnaire constructor in order to record the URL from the questionnaire.<br>
 It is hidden from the users.<br>
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemInvisible
 @augments QuestionnaireItemHidden
 */
QuestionnaireItemHiddenURL = function () {
    QuestionnaireItemHidden.call(this, "URL", window.location.href);
}
QuestionnaireItemHiddenURL.prototype = Object.create(QuestionnaireItemHidden.prototype);
QuestionnaireItemHiddenURL.prototype.constructor = QuestionnaireItemHiddenURL;

/**
 Questionnaire item that waits a minimal number of seconds before setting itself
 ready (allowing to go to next screen) OR goes to next screen automatically
 after a maximum number of seconds. <br>
 Observe that after the maximum time the next screen will be called even if all
 the required QuestionnaireItems have not been answered yet! <br>
 In case of wanting both minimal and maximum times, the element should be
 implemented twice.<br>
 It is not displayable to the user.
 @class QuestionnaireItemWait
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemInvisible
 @param {number} waitTime waiting time in seconds
 @param {boolean} maximalWait false for minimal waiting time and true for maximum waiting time

 TODO Actually this is not a questionnaireItem in sense of inheritance, but it gives an answer.
 */
function QuestionnaireItemWait(waitTime, maximalWait) {
    QuestionnaireItemInvisible.call(this, null, "", true);
    this.maximalWait = Boolean(maximalWait);
    this.waitTime = !isNaN(waitTime) ? Math.abs(waitTime) * 1000 : 0;

    this.timeoutHandle = null;

    TheFragebogen.logger.debug("QuestionnaireItemWait()", "Set: waitTime as " + this.waitTime);
}
QuestionnaireItemWait.prototype = Object.create(QuestionnaireItemInvisible.prototype);
QuestionnaireItemWait.prototype.constructor = QuestionnaireItemWait;
/**
 Defines if the element is required.<br>
 Sets the timeout.
 */
QuestionnaireItemWait.prototype.createUI = function () {
    this.required = !this.maximalWait;
    this.timeoutHandle = setTimeout((this._waitTimeCallback).bind(this), this.waitTime);
};
/**
 Sets the element ready or goes to next page.
 */
QuestionnaireItemWait.prototype._waitTimeCallback = function () {
    this.maximalWait ? document.dispatchEvent(new Event("nextScreen")) : this.required = false;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemWait.prototype.releaseUI = function () {
    if (this.timeoutHandle != null) {
        clearTimeout(this.timeoutHandle);
        this.timeoutHandle == null;
    }
};
/**
 Returns the waiting time and type (minimal or maximum time) of the element.
 @returns {array} an array containing the waiting time in the index 0 and the type in the index 1
 */
QuestionnaireItemWait.prototype.getData = function () {
    return [this.waitTime, this.maximalWait];
};
/**
 Compares the waiting time and the type (minimal or maximum time) of the provided
 input data array with the waiting time and the type stored in the element.
 @param {array} data waiting time (index 0) and type (index 1) stored in the array
 @returns {boolean} true if it is the same waiting time and the same type, false if not
 */
QuestionnaireItemWait.prototype._checkData = function (data) {
    return (data[0] == this.waitTime && data[1] == this.maximalWait);
};
/**
 Calls the method _checkData() as no setData() is available for this type of element.
 @param {array} data waiting time (index 0) and type (index 1) stored in the array
 @returns {boolean} same as _checkData() returns: true if it is the same waiting time and the same type, false if not
 */
QuestionnaireItemWait.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    return true;
};

/**
 Questionnaire item for small text answers like names.<br>
 This item uses only one line of a fixed size.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <input type="text">
 </div>
 </div>
 @class QuestionnaireItemText
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemText(className, question, required) {
    QuestionnaireItem.call(this, className, question, required);

    this.input = null;
}
QuestionnaireItemText.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemText.prototype.constructor = QuestionnaireItemText;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemText.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.input = document.createElement("input");
    this.input.addEventListener("change", this._handleChange.bind(this));

    node.appendChild(this.input);

    this._applyAnswerToUI();
    return node;
};
//TODO documentation of all _handleChange() functions
QuestionnaireItemText.prototype._handleChange = function (event) {
    this.answer = this.input.value;

    TheFragebogen.logger.info("QuestionnaireItemText._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemText.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;


    if (this.isAnswered())
        this.input.value = this.getAnswer();
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemText.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    this.answer = answer;
    this._applyAnswerToUI();
    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemText.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = null;
};
/**
 Returns the question and answer of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemText.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()]
};
/**
 Compares the question of the provided input data array and the question stored
 in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question, false if not
 */
QuestionnaireItemText.prototype._checkData = function (data) {
    return (data[0] == this.question);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemText.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};

/**
 This a class of questionnaire items that have predefined answer options
 provided by the questionnaire constructor.<br>
 It is by default: enabled = false.<br>
 The elements of this class demand a question and list of answer options.
 @abstract
 @class QuestionnaireItemDefined
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {array} optionList cointains strings which are the possible options
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemDefined(className, question, optionList, required) {
    QuestionnaireItem.call(this, className, question, required);

    this.optionList = (optionList instanceof Array ? optionList : ["a", "b"
    ]); //TODO change the optionList checking
    this.input = [];

    TheFragebogen.logger.debug("QuestionnaireItemDefined()", "Set: optionList as " + this.optionList);
}
QuestionnaireItemDefined.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemDefined.prototype.constructor = QuestionnaireItem;
/**
 Returns the list of answer options.
 @returns {array} array of strings containing the possible options
 */
QuestionnaireItemDefined.prototype.getOptionList = function () {
    return this.optionList;
};
/**
 Sets the QuestionnaireItemDefined enabled.
 @param {boolean} enable if the item must be enabled
 */
QuestionnaireItemDefined.prototype.setEnabled = function (enable) {
    this.enabled = this.isUIcreated() ? enable : false;
 
    if (this.input instanceof Array) {
        for (var i in this.input)
            this.input[i].disabled = !this.enabled;
    }
};

/**
 Questionnaire item that has a list of answer options provided by the
 questionnaire cronstuctor and accept only one option as answer.<br>
 This item uses radio buttons.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <table class="QuestionnaireItemDefinedOne">
 <tbody>
 <tr>
 //One 'td' here for each answer option
 <td>
 <label>Answer option</label>
 </td>
 </tr>
 <tr>
 //One 'td' here for each answer option
 <td>
 <input type="radio">
 </td>
 </tr>
 </tbody>
 </table>
 </div>
 @class QuestionnaireItemDefinedOne
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemDefined
 @param {string} [className] CSS-style
 @param {string} question question
 @param {array} optionList cointains strings which are the possible options
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemDefinedOne(className, question, options, required) {
    QuestionnaireItemDefined.call(this, className, question, options, required);

    this.identifier = Math.random(); //Part of the identifier for the label + radiobutton relation.
}
QuestionnaireItemDefinedOne.prototype = Object.create(QuestionnaireItemDefined.prototype);
QuestionnaireItemDefinedOne.prototype.constructor = QuestionnaireItemDefinedOne;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemDefinedOne.prototype._createAnswerNode = function () {
    var tableRowLabel = document.createElement('tr');
    var tableRowRadio = document.createElement('tr');

    for (var i = 0; i < this.optionList.length; i++) {
        this.input[i] = document.createElement("input");
        this.input[i].id = this.identifier + i;
        this.input[i].name = this.identifier;
        this.input[i].type = "radio";
        this.input[i].value = i;
        if (this.answer === this.optionList[i]) this.input[i].checked = true;

        this.input[i].addEventListener("change", this._handleChange.bind(this));

        var label = document.createElement("label");
        label.setAttribute("for", this.identifier + i);
        label.innerHTML = this.optionList[i];

        var tdLabel = document.createElement('td');
        tdLabel.appendChild(label);
        tableRowLabel.appendChild(tdLabel);

        var tdRadio = document.createElement('td');
        tdRadio.appendChild(this.input[i]);
        tableRowRadio.appendChild(tdRadio);
    }

    var tableBody = document.createElement('tbody');
    tableBody.appendChild(tableRowLabel);
    tableBody.appendChild(tableRowRadio);

    var table = document.createElement('table');
    table.className = "QuestionnaireItemDefinedOne";
    table.appendChild(tableBody);
    return table;
};
QuestionnaireItemDefinedOne.prototype._handleChange = function (event) {
    this.answer = this.optionList[event.target.value];
    this.markRequired();
    this._sendReadyCallback();
    TheFragebogen.logger.info("QuestionnaireItemDefinedOne._handleChange()", this.answer);
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemDefinedOne.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;

    for (var i = 0; i < this.answer.length; i++)
        if (this.input[i] != undefined) {
            this.input[i].checked = this.answer[i] || false;
        }
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemDefinedOne.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    var answerIndex = this.optionList.indexOf(answer);
    if (answerIndex == -1) {
        TheFragebogen.logger.error("QuestionnaireItemDefinedOne.setAnswer()", "Provided answer is not an option " + answer + ".");
        return false;
    }

    this.answer = this.optionList[answerIndex];
    this._applyAnswerToUI();

    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemDefinedOne.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = [];
    this.identifier = null;
};
/**
 Returns the question, list of answer options and answer of the element.
 @returns {array} an array containing the question in the index 0, the list of answer options in the index 1 and the answer in the index 2
 */
QuestionnaireItemDefinedOne.prototype.getData = function () {
    return [this.getQuestion(), this.optionList, this.getAnswer()];
};
/**
 Compares the question and the list of answer options of the provided input data
 array with the question and the list of answer options stored in the element.
 @param {array} data question (index 0) and list of answer options (array of strings containing the possible options in the index 1) stored in the array
 @returns {boolean} true if it is the same question and the same list of answer options, false if not
 */
QuestionnaireItemDefinedOne.prototype._checkData = function (data) {
    return (data[0] == this.question) && (JSON.stringify(data[1]) == JSON.stringify(this.optionList));
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question and the list of answer options of the provided input data must be
 the same as the question and the list of answer options stored in the element.
 @param {array} data question, list of answer options and answer stored in the indexes 0, 1 and 2 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemDefinedOne.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[2]);
    return true;
};

/**
 Questionnaire item that has a list of answer options provided by the
 questionnaire cronstuctor and accept as many answers as the user wishes.<br>
 This item uses checkboxes.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 //A pair of 'input' and 'label' here for each answer option
 <input type="checkbox">
 <label>Answer option</label>
 </div>
 </div>
 @class QuestionnaireItemDefinedMulti
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemDefined
 @param {string} [className] CSS-style
 @param {string} question question
 @param {array} optionList cointains strings which are the possible options
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemDefinedMulti(className, question, optionList, required) {
    QuestionnaireItemDefined.call(this, className, question, optionList, required);

    this.identifier = Math.random(); //Part of the identifier for the label + checkbox relation.
    this.answer = [];
}
QuestionnaireItemDefinedMulti.prototype = Object.create(QuestionnaireItemDefined.prototype);
QuestionnaireItemDefinedMulti.prototype.constructor = QuestionnaireItemDefinedMulti;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemDefinedMulti.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    for (var i = 0; i < this.optionList.length; i++) {
        this.input[i] = document.createElement("input");
        this.input[i].id = this.identifier + i;
        this.input[i].name = this.identifier;
        this.input[i].type = "checkbox";
        this.input[i].value = i;

        this.input[i].addEventListener("change", this._handleChange.bind(this));

        var label = document.createElement("label");
        label.setAttribute("for", this.identifier + i);
        label.innerHTML = this.optionList[i];

        node.appendChild(this.input[i]);
        node.appendChild(label);
    }

    this._applyAnswerToUI();
    return node;
};
QuestionnaireItemDefinedMulti.prototype._handleChange = function (event) {
    this.answer[event.target.value] = event.target.checked;

    TheFragebogen.logger.info("QuestionnaireItemDefinedMulti._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemDefinedMulti.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;

    for (var i = 0; i < this.answer.length; i++)
        if (this.input[i] != undefined) {
            this.input[i].checked = this.answer[i] || false;
        }
};
//TODO documentation, the function need paramater? it is called without it.
QuestionnaireItemDefinedMulti.prototype.getAnswer = function (answer) {
    var result = this.optionList.slice(0); //Clone answer

    for (var i = 0; i < this.optionList.length; i++) //Remove not selected items
        if (!this.answer[i]) result[i] = null;

    return result.filter(function (n) {
        return n != null;
    });
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemDefinedMulti.prototype.setAnswer = function (answer) { //answer is either string or array[string]
    if (answer === null) {
        this.answer = [];
        this._applyAnswerToUI();
        return true;
    }

    if (typeof (answer) === "string") answer = [answer];

    if (answer instanceof Array) {
        this.answer = [];

        if (answer.length > this.optionList.length) {
            TheFragebogen.logger.warn("QuestionnaireItemDefinedMulti.setAnswer()", "use only an array.")
            return false;
        }

        for (var i = 0; i < answer.length; i++) {
            var optionIndex = this.optionList.indexOf(answer[i]);
            if (optionIndex == -1) {
                TheFragebogen.logger.warn("QuestionnaireItemDefinedMulti.setAnswer()", "Option " + answer[i] + " is not available in " + this.optionList + ".");
                this.answer = [];
                return false;
            }
            this.answer[optionIndex] = true;
        }
        this._applyAnswerToUI();
        this._sendReadyCallback();
        return true;
    }

    TheFragebogen.logger.warn("QuestionnaireItemDefinedMulti.setAnswer()", "Use either string or array[string].");
    return false;
};
QuestionnaireItemDefinedMulti.prototype.isAnswered = function () {
    return this.answer.length > 0;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemDefinedMulti.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = [];
    this.identifier = null;
};
/**
 Returns the question, list of answer options and answer of the element.
 @returns {array} an array containing the question in the index 0, the list of answer options in the index 1 and the answer in the index 2
 */
QuestionnaireItemDefinedMulti.prototype.getData = function () {
    return [this.getQuestion(), this.optionList, this.getAnswer()];
};
/**
 Compares the question and the list of answer options of the provided input data
 array with the question and the list of answer options stored in the element.
 @param {array} data question (index 0) and list of answer options (array of strings containing the possible options in the index 1) stored in the array
 @returns {boolean} true if it is the same question and the same list of answer options, false if not
 */
QuestionnaireItemDefinedMulti.prototype._checkData = function (data) {
    return (data[0] == this.question) && (JSON.stringify(data[1]) == JSON.stringify(this.optionList));
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question and the list of answer options of the provided input data must be
 the same as the question and the list of answer options stored in the element.
 @param {array} data question, list of answer options and answer stored in the indexes 0, 1 and 2 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemDefinedMulti.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[2]);
    return true;
};

/**
 Questionnaire item for free-hand input using a mouse or a tablet stylus pen.<br>
 It is also possible to erase using the right button of the mouse for example.<br>
 ATTENTION: This UI element cannot be re-used! TODO: what does it mean?<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <canvas class="QuestionnaireItemWrite">
 </div>
 </div>
 @class QuestionnaireItemWrite
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] true if the item must be answered, false if not
 @param {string} [backgroundImg] image in the background
 @param {number} [height=240] image height
 @param {number} [width=320] image width
 @param {number} [drawSize=1] pixels of the pen
 @param {number} [eraserSize=10] pixels of the eraser
 */
function QuestionnaireItemWrite(className, question, required, backgroundImg, width, height, drawColor, drawSize, eraserSize) {
    QuestionnaireItem.call(this, className, question, required);

    this.className = (typeof className === "string" ? className : "");
    this.backgroundImg = (typeof backgroundImg === "string" ? backgroundImg : null);
    this.height = !isNaN(height) ? height : 240;
    this.width = !isNaN(width) ? width : 320;

    this.ratio = 1;

    this.drawColor = (typeof drawColor === "string" ? drawColor : "black");
    this.drawSize = !isNaN(drawSize) ? drawSize : 1;
    this.eraserSize = !isNaN(eraserSize) ? eraserSize : 10;

    TheFragebogen.logger.debug("QuestionnaireItemWrite", "Set: backgroundImg as " + this.backgroundImg + ", height as " + this.height + ", width as " + this.width + ", drawColor as " + this.drawColor + ", drawSize as " + this.drawSize + " and eraserSize as " + this.eraserSize);

    this.painting = false;
    this.penDown = false;
    this.eraserMode = false; //True: eraser, False: draw

    this.lastDrawX = null;
    this.lastDrawY = null;

    this.context = null;
}
QuestionnaireItemWrite.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemWrite.prototype.constructor = QuestionnaireItemWrite;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemWrite.prototype._createAnswerNode = function () {
    var node = document.createElement("div");
    var canvas = document.createElement("canvas");
    if (this.width != null) canvas.width = this.width;
    if (this.height != null) canvas.height = this.height;
    node.appendChild(canvas);

    canvas.className = "QuestionnaireItemWrite";

    this.context = canvas.getContext("2d");
    this.context.lineJoin = "round";

    if (this.backgroundImg != null)
        canvas.style.background = "url('" + this.backgroundImg + "') 50% 50% / contain no-repeat";

    if (this.answer != null) {
        var img = new Image();
        img.src = this.answer;
        this.context.drawImage(img, 0, 0);
    }

    canvas.onmousedown = (this.onWritingStart).bind(this);
    canvas.onmousemove = (this.onWriting).bind(this);
    canvas.onmouseup = (this.onWritingStop).bind(this);
    canvas.onmouseout = (this.onWritingStop).bind(this);

//BEGIN: EXPERIMENTAL
//This uses allows us to be HDPI conform!
//Only works in Chrome so far! And it is a hack! See: http://www.html5rocks.com/en/tutorials/canvas/hidpi/

    this.ratio = window.devicePixelRatio || 1 / this.context.webkitBackingStorePixelRatio || 1;

    canvas.style.width = canvas.width;
    canvas.style.height = canvas.height;

    canvas.width = canvas.width * this.ratio;
    canvas.height = canvas.height * this.ratio;

    this.context.scale(this.ratio, this.ratio);

//END: EXPERIMENTAL

    return node;
};
/**
 Starts writing by clicking the mouse button inside of the item.
 */
QuestionnaireItemWrite.prototype.onWritingStart = function (event) {
    if (!this.isEnabled()) return;

    this.painting = true;
    this.eraserMode = event.button != 0; //Not-left mouse button is the eraser!
    this.penDown = false;

    this.onWriting(event);
};
/**
 Writes moving the mouse with the button pressed.
 */
QuestionnaireItemWrite.prototype.onWriting = function (event) {
    if (!this.isEnabled() || !this.painting) return;

    var x = event.pageX - event.target.offsetLeft;
    var y = event.pageY - event.target.offsetTop;

    this.context.beginPath();

    if (this.eraserMode) {
        this.context.globalCompositeOperation = "destination-out";
        this.context.arc(x, y, this.eraserSize, 0, Math.PI * 2, false);
        this.context.fill();
    }
    else {
        this.context.globalCompositeOperation = "source-over";
        if (this.penDown) this.context.moveTo(this.lastDrawX, this.lastDrawY);
        else this.context.moveTo(x - 1, y);

        this.context.lineTo(x, y);
        this.context.strokeStyle = this.drawColor;
        this.context.lineWidth = this.drawSize;
        this.context.stroke();
    }

    //The following lines cannot be put above, because it must be done after the draw.
    this.penDown = true;
    this.lastDrawX = x;
    this.lastDrawY = y;

    //TO REMEMBER: this.context.getImageData(0,0,this.context.canvas.width,this.context.canvas.height);
    //TO REMEMBER: this.context.canvas.toDataURL("image/png").replace("image/png", "image/octet-stream");
};
/**
 Stops writing and saves the answer.
 */
QuestionnaireItemWrite.prototype.onWritingStop = function () {
    if (this.painting)
        this.answer = this.context.canvas.toDataURL("image/png");
    this.painting = false;

    if (this.isAnswered()) this.markRequired();
    this._sendReadyCallback();
};
/**
 Sets the QuestionnaireItemDefined enabled.
 @param {boolean} enable if the item must be enabled
 */
QuestionnaireItemWrite.prototype.setEnabled = function (enable) {
    this.enabled = this.isUIcreated() ? enable : false;
    if (!this.isEnabled()) this.painting = false;
};
/**
 Gets the answer of the item.
 @returns {string} answer
 */
QuestionnaireItemWrite.prototype.getAnswer = function () {
    if (this.isUIcreated() && this.isAnswered()) {
        this.context.canvas.style.width *= this.ratio;
        this.context.canvas.style.height *= this.ratio;
        this.answer = this.context.canvas.toDataURL("image/png");
        this.context.canvas.style.width /= this.ratio;
        this.context.canvas.style.height /= this.ratio;
    }

    return this.answer;
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemWrite.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        if (this.isUIcreated())
            this.context.clearRect(0, 0, this.context.canvas.width, this.context.canvas.height);
        return true;
    }
    if (typeof (answer) === "string") {
        this.answer = answer;
        if (this.isUIcreated()) {
            this.context.clearRect(0, 0, this.context.canvas.width, this.context.canvas.height);
            var img = new Image();
            img.src = answer;

            var ratio_w = img.width / parseInt(this.context.canvas.style.width);
            var ratio_h = img.height / parseInt(this.context.canvas.style.height);

            this.context.canvas.style.width *= ratio_w;
            this.context.canvas.style.height *= ratio_h;
            this.context.scale(1 / ratio_w, 1 / ratio_h);

            this.context.drawImage(img, 0, 0);

            this.context.canvas.style.width /= ratio_w;
            this.context.canvas.style.height /= ratio_h;
            this.context.scale(ratio_w, ratio_h);
        }
        this._sendReadyCallback();
        return true;
    }
    TheFragebogen.logger.warn("QuestionnaireItemWrite.setAnswer()", "Invalid answer: " + answer + ".");
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemWrite.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.getAnswer();

    this.context = null;
    this.ratio = 1;
    this.lastDrawX = null;
    this.lastDrawY = null;
    this.penDown = false;
    this.painting = false;
    this.eraserMode = false;
};
/**
 Returns the question and answer of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemWrite.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
/**
 Compares the question of the provided input data array and the question stored
 in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question, false if not
 */
QuestionnaireItemWrite.prototype._checkData = function (data) {
    return data[0] == this.question;
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemWrite.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};

/**
 Questionnaire item for date selection.<br>
 The item uses date picker and text field.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <input type="date">
 </div>
 </div>
 @class QuestionnaireItemDate
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] true if the item must be answered, false if not
 @param {string} [min] minimum date
 @param {string} [max] maximum date
 @param {string} [pattern] date pattern
 */
function QuestionnaireItemDate(className, question, required, min, max, pattern) {
    QuestionnaireItem.call(this, className, question, required);

    this.min = (typeof min === "string" ? min : null);
    this.max = (typeof max === "string" ? max : null);
    this.pattern = (typeof pattern === "string" ? pattern : null);

    TheFragebogen.logger.debug("QuestionnaireItemDate()", "Set: min as " + this.min + ", max as " + this.max + " and pattern as " + this.pattern);

    this.input = null;
}
QuestionnaireItemDate.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemDate.prototype.constructor = QuestionnaireItemDate;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemDate.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.input = document.createElement("input");
    this.input.type = "date";
    this.input.min = this.min;
    this.input.max = this.max;
    this.input.pattern = this.pattern;
    this.input.addEventListener("change", this._handleChange.bind(this));

    node.appendChild(this.input);

    this._applyAnswerToUI();
    return node;
};
QuestionnaireItemDate.prototype._handleChange = function (event) {
    this.answer = this.input.value;

//    TheFragebogen.logger.info("QuestionnaireItemDate._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemDate.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;


    if (this.isAnswered())
        this.input.value = this.getAnswer();
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemDate.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    this.answer = answer;
    this._applyAnswerToUI();
    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemDate.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = null;
};
/**
 Returns the question, date pattern and answer of the element.
 @returns {array} an array containing the question in the index 0, the date pattern in the index 1 and the answer in the index 2
 */
QuestionnaireItemDate.prototype.getData = function () {
    return [this.getQuestion(), this.pattern, this.getAnswer()]
};
/**
 Compares the question and the date pattern of the provided input data array
 with the question and the date pattern stored in the element.
 @param {array} data question (index 0) and date pattern (index 1) stored in the array
 @returns {boolean} true if it is the same question and the same date pattern, false if not
 */
QuestionnaireItemDate.prototype._checkData = function (data) {
    return (data[0] == this.question) && (data[1] == this.pattern);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question and the date pattern of the provided input data must be
 the same as the question and the date pattern stored in the element.
 @param {array} data question, date pattern and answer stored in the indexes 0, 1 and 2 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemDate.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[2]);
    return true;
};

/**
 Questionnaire item that has a range of integers provided by the questionnaire
 cronstuctor and accept as answer a value between the range ends.<br>
 This item uses a slider.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <input type="range">
 </div>
 </div>
 @class QuestionnaireItemDefinedRange
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemDefined
 @param {string} [className] CSS-style
 @param {string} question question
 @param {array} [optionList] cointains the ends of the range (must be numbers)
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemDefinedRange(className, question, min, max, required) {
    QuestionnaireItemDefined.call(this, className, question, [min, max
    ], required);

    this.min = min;
    this.max = max;

    this.input = null;
}
QuestionnaireItemDefinedRange.prototype = Object.create(QuestionnaireItemDefined.prototype);
QuestionnaireItemDefinedRange.prototype.constructor = QuestionnaireItemDefinedRange;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemDefinedRange.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.input = document.createElement("input");
    this.input.type = "range";
    this.input.min = this.min;
    this.input.max = this.max;
    this.input.addEventListener("change", this._handleChange.bind(this));

    node.appendChild(this.input);

    this._applyAnswerToUI();
    return node;
};
QuestionnaireItemDefinedRange.prototype._handleChange = function (event) {
    this.answer = this.input.value;

//    TheFragebogen.logger.info("QuestionnaireItemDefinedRange._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemDefinedRange.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;


    if (this.isAnswered())
        this.input.value = this.getAnswer();
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemDefinedRange.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    this.answer = answer;
    this._applyAnswerToUI();
    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemDefinedRange.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = null;
};
/**
 Returns the question, range and answer of the element.
 @returns {array} an array containing the question in the index 0, the minimum and maximum (indexes 0 and 1) of the range on an array inside the index 1 and the answer in the index 2
 */
QuestionnaireItemDefinedRange.prototype.getData = function () {
    return [this.getQuestion(), [this.min, this.max], this.getAnswer()];
};
/**
 Compares the question and the range of the provided input data array with the
 question and the range stored in the element.
 @param {array} data question (index 0), minimum (index 0 of the array in the index 1) and maximum (index 1 of the array in the index 1) stored in the array
 @returns {boolean} true if it is the same question and the same range, false if not
 */
QuestionnaireItemDefinedRange.prototype._checkData = function (data) {
    return (data[0] == this.question) && (data[1][0] == this.min) && (data[1][1] == this.max);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question and the range of the provided input data must be
 the same as the question and the range stored in the element.
 @param {array} data question, range and answer stored in the indexes 0, 1 and 2 of the array;
 the range is an array with the minimum in the index 1 and the maximum in the index 2
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemDefinedRange.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[2]);
    return true;
};

/**
 Questionnaire item for free text input.<br>
 This item uses text area and it is resizable.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <textarea></textarea>
 </div>
 </div>
 @class QuestionnaireItemTextArea
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] true if the item must be answered, false if not
 @param {number} [rows=2] number of rows
 @param {number} [cols=19] number of collumns
 @param {string} placehnewer
 */
//TODO placehnewer: description. not optional and without default?
function QuestionnaireItemTextArea(className, question, required, rows, cols, placehnewer) {
    QuestionnaireItem.call(this, className, question, required);

    this.rows = !isNaN(rows) ? rows : 2;
    this.cols = !isNaN(cols) ? cols : 19;
    this.placehnewer = (typeof placehnewer === "string" ? placehnewer : null);

    TheFragebogen.logger.debug("QuestionnaireItemTextArea()", "Set: rows as " + this.rows + ", cols as " + this.cols + " and placehnewer as " + this.placehnewer);

    this.textarea = null;
}
QuestionnaireItemTextArea.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemTextArea.prototype.constructor = QuestionnaireItemTextArea;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemTextArea.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.textarea = document.createElement("textarea");
    this.textarea.rows = this.rows;
    this.textarea.cols = this.cols;
    this.textarea.placehnewer = this.placehnewer;
    this.textarea.addEventListener("change", this._handleChange.bind(this));

    node.appendChild(this.textarea);

    this._applyAnswerToUI();
    return node;
};
QuestionnaireItemTextArea.prototype._handleChange = function (event) {
    this.answer = this.textarea.value;

//    TheFragebogen.logger.info("QuestionnaireItemTextArea._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemTextArea.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;


    if (this.isAnswered())
        this.textarea.value = this.getAnswer();
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemTextArea.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    this.answer = answer;
    this._applyAnswerToUI();
    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemTextArea.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.textarea = null;
};
/**
 Returns the question and answer of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemTextArea.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()]
};
/**
 Compares the question of the provided input data array and the question stored
 in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question, false if not
 */
QuestionnaireItemTextArea.prototype._checkData = function (data) {
    return (data[0] == this.question);
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemTextArea.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};

/**
 Questionnaire item that has a list of answer options provided by the
 questionnaire cronstuctor and accept only one option as answer.<br>
 This item uses a popup selection box.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div>
 <select>
 //One 'option' here for each answer option
 <option>Answer option</option>
 </select>
 </div>
 </div>
 @class QuestionnaireItemDefinedSelector
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @augments QuestionnaireItemDefined
 @param {string} [className] CSS-style
 @param {string} question question
 @param {array} optionList cointains strings which are the possible options
 @param {boolean} [required=false] true if the item must be answered, false if not
 */
function QuestionnaireItemDefinedSelector(className, question, optionList, required) {
    QuestionnaireItemDefined.call(this, className, question, optionList, required);
}
QuestionnaireItemDefinedSelector.prototype = Object.create(QuestionnaireItemDefined.prototype);
QuestionnaireItemDefinedSelector.prototype.constructor = QuestionnaireItemDefinedSelector;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemDefinedSelector.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.select = document.createElement("select");
    this.select.addEventListener("change", this._handleChange.bind(this));

    for (var i in this.optionList) {
        var option = document.createElement("option");
        option.value = this.optionList[i];
        option.innerHTML = this.optionList[i];
        this.select.appendChild(option);
    }

    node.appendChild(this.select);

    this._applyAnswerToUI();
    return node;
};
QuestionnaireItemDefinedSelector.prototype._handleChange = function (event) {
    this.answer = this.select.value;
//    TheFragebogen.logger.info("QuestionnaireItemSelector._handleChange()", this.getAnswer() + ".");
};
/**
 Applies the set answer to the UI.
 */
QuestionnaireItemDefinedSelector.prototype._applyAnswerToUI = function () {
    if (!this.isUIcreated()) return;

    if (this.isAnswered())
        this.select.value = this.getAnswer();
};
/**
 Sets the answer.
 @param {string} answer answer
 @returns {boolean} true if the answer was set, false if not
 */
QuestionnaireItemDefinedSelector.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._applyAnswerToUI();
        return true;
    }

    var answerIndex = this.optionList.indexOf(answer);
    if (answerIndex == -1) {
        TheFragebogen.logger.error("QuestionnaireItemDefinedSelector.setAnswer()", "Provided answer is not an option " + answer + ".");
        return false;
    }

    this.answer = this.optionList[answerIndex];
    this._applyAnswerToUI();

    this._sendReadyCallback();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemDefinedSelector.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.input = [];
    this.select = null;
};
/**
 Returns the question, list of answer options and answer of the element.
 @returns {array} an array containing the question in the index 0, the list of answer options in the index 1 and the answer in the index 2
 */
QuestionnaireItemDefinedSelector.prototype.getData = function () {
    return [this.getQuestion(), this.optionList, this.getAnswer()];
};
/**
 Compares the question and the list of answer options of the provided input data
 array with the question and the list of answer options stored in the element.
 @param {array} data question (index 0) and list of answer options (array of strings containing the possible options in the index 1) stored in the array
 @returns {boolean} true if it is the same question and the same list of answer options, false if not
 */
QuestionnaireItemDefinedSelector.prototype._checkData = function (data) {
    return (data[0] == this.question) && (JSON.stringify(data[1]) == JSON.stringify(this.optionList));
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question and the list of answer options of the provided input data must be
 the same as the question and the list of answer options stored in the element.
 @param {array} data question, list of answer options and answer stored in the indexes 0, 1 and 2 of the array
 @returns {boolean} true if the input data answer was set, false if not
 */
QuestionnaireItemDefinedSelector.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[2]);
    return true;
};
/**

 Coordinates the sequence of screens to show and does the lifecycle management for screens.<br>
 <br>
 Constructor parameters: array of Screens.<br>
 <br>
 Handles the following events:<br>
 * nextScreen<br>
 * requestdataCSV<br>
 * requestdataLS<br>
 * goToScreen<br>
 * restoreSession<br>
 <br>
 * (optional) HTML5 Localstorage:<br>
 If LocalStorage is available, intermediate data is stored and destroyed some milliseconds after the last screen is shown.
 @class ScreenController
 */
function ScreenController() {
    if (arguments.length == 0)
        TheFragebogen.logger.fatal("ScreenController.constructor", "No screen available. Cannot proceed with the questionnaire");

    this.screen = arguments;

    this.currentScreenIndex = null;
    this.screenContainerNode = null;

    document.addEventListener("nextScreen", (this.next).bind(this));
    document.addEventListener("goToScreen", (this._goToScreenEvent).bind(this));
    document.addEventListener("requestDataCSV", (this.requestDataCSV).bind(this));
    document.addEventListener("requestDataLS", (this.requestDataLS).bind(this));
    document.addEventListener("restoreSession", (this.restoreSession).bind(this));
}
ScreenController.prototype.constructor = ScreenController;
/**
 Initialites the ScreenController.
 This includes restoring from LocalStorage.
 */
ScreenController.prototype.init = function (parentNode) {
    if (this.screenContainerNode !== null) {
        TheFragebogen.logger.warn("ScreenController.init()", "Called, but is already initialized.");
        return;
    }

    TheFragebogen.logger.debug("ScreenController.init()", "Start");

    this.screenContainerNode = document.createElement("div");
    this.screenContainerNode.setAttribute("class", "screenContainer");
    parentNode.innerHTML = null;
    parentNode.appendChild(this.screenContainerNode);

    this.currentScreenIndex = 0;

    if (TheFragebogen.logger == null || TheFragebogen.logger instanceof LogConsole) {
        TheFragebogen.logger = new LogWindow(TheFragebogen.logger.logMessages != null ? TheFragebogen.logger.logMessages : null);
    }
};
ScreenController.prototype.start = function (node) {
    this._displayUI(node);
};
/**
 Proceeds to the next Screen, if the current is ready.
 */
ScreenController.prototype.next = function () {
    if (this.screenContainerNode === null) {
        TheFragebogen.logger.error("ScreenController.next()", "Please call init() before.");
        return;
    }

    if (this.isLastScreen()) {
        TheFragebogen.logger.warn("ScreenController.next()", "No further screen available.");
        return;
    }

    if (this.screen[this.currentScreenIndex].isReady ? this.screen[this.currentScreenIndex].isReady() : false) {
        this.screen[this.currentScreenIndex].releaseUI();
        this.screenContainerNode.innerHTML = null;

        this.currentScreenIndex++;
        this._displayUI();
    }
};
ScreenController.prototype._displayUI = function () {
    if (this.currentScreenIndex >= this.screen.length) {
        TheFragebogen.logger.error("ScreenController._displayUI()", "There is no screen with index " + this.currentScreenIndex + ".");
        return;
    }

    TheFragebogen.logger.info("ScreenController._displayUI()", "Displaying next screen with index: " + this.currentScreenIndex + ".");

    //Scroll back to top
    window.scrollTo(0, document.body.scrollLeft);

    //Add the new screen
    var screen = this.screen[this.currentScreenIndex];
    this.screenContainerNode.appendChild(screen.createUI());
    screen.start();
};
/**
 Prepare data for export: at the moment this means CSV:<br>
 * Line 1: ScreenIndex<br>
 * Line 2: Questions<br>
 * Line 3: Answers<br>
 If we are on the last screen, localStorageStop() is called.
 */
ScreenController.prototype.requestDataCSV = function (event) {
    if (!(event.callback instanceof Function)) {
        TheFragebogen.logger.info("ScreenController.requestDataCSV()", "Please provide event.callback with valid function handle.");
        return;
    }

    TheFragebogen.logger.info("ScreenController.requestDataCSV()", "called.");

    var screenIndeces = ["Screen index"];
    var questionType = ["Type of question"];
    var questions = ["Question"];
    var options = ["Answer options"];
    var answers = ["Answer"];

    for (var i = 0; i <= this.currentScreenIndex; i++) {
        var currentData = this.screen[i].getDataCSV();
        if (currentData instanceof Array && currentData[0] instanceof Array && currentData[1] instanceof Array && currentData[2] instanceof Array && currentData[3] instanceof Array) {
            if (currentData[0].length == 0) continue;

            if (currentData[1].length > currentData[3].length) {
                TheFragebogen.logger.warn("ScreenController.requestDataCSV()", "More Question than answer - filling with null.");
                currentData[1][currentData[0].length] = null;
            }
            screenIndeces = screenIndeces.concat(Array.apply(null, new Array(currentData[0].length)).
                    map(Number.prototype.valueOf, i));
            questionType = questionType.concat(currentData[0]);
            questions = questions.concat(currentData[1]);
            options = options.concat(currentData[2]);
            answers = answers.concat(currentData[3]);
        }
    }
    for (var i in screenIndeces) {
        if (result == null)
            var result = '"' + screenIndeces[i] + '","' + questionType[i] + '","' + questions[i] + '","' + options[i] + '","' + answers[i] + '"\n';
        else
            result += '"' + screenIndeces[i] + '","' + questionType[i] + '","' + questions[i] + '","' + options[i] + '","' + answers[i] + '"\n';
    }
    event.callback(result);
};
/**
 Return a boolean. True if the actual screen is the last one, else false.
 */
ScreenController.prototype.isLastScreen = function () {
    return !(this.currentScreenIndex < this.screen.length - 1);
}

/**
 Prepare data to update the localStorage with all current values.
 */
ScreenController.prototype.requestDataLS = function (event) {
    if (!(event.callback instanceof Function)) {
        TheFragebogen.logger.info("ScreenController.requestDataLS()", "Please provide event.callback with valid function handle.");
        return;
    }
    TheFragebogen.logger.info("ScreenController.requestDataLS()", "Called.");

    var data = [];
    for (var i = 0; i < this.screen.length; i++) {
        data.push(this.screen[i].getData());
    }

    var currentScreenIndex = this.currentScreenIndex;

    event.callback(data, currentScreenIndex);
};

/**
 Restore the current session, which means restore data and currentScreenIndex
 */
ScreenController.prototype.restoreSession = function (event) {
    if (event.currentScreenIndex >= this.screen.length) {
        TheFragebogen.logger.warn("ScreenController.restoreSession()", "Cannot forward to " + (data.currentScreenIndex + 1) + ", not enough screens available.");
        return;
    }
    for (var i = 0; i <= event.currentScreenIndex; i++) {
        if (!this.screen[i].setData(event.screenData[i])) {
            this.currentScreenIndex = i;
            //TODO Should delete setData on all previous elements, i.e. setData(null)
            TheFragebogen.logger.warn("ScreenController.restoreSession()", "Data restored until screen " + i + ".");
            return false;
        }
    }
    this.currentScreenIndex = event.currentScreenIndex;
    TheFragebogen.logger.info("ScreenController.restoreSession()", "Successful.");
    return true;
};
/**
 Return the current screen.
 */
ScreenController.prototype.getCurrentScreen = function () {
    return this.screen[this.currentScreenIndex];
};
/**
 Go to a defined screen immediately. Use number.
@argument {number} screenId The id of the screen that should be displayed.
@return {boolean} 
 */
ScreenController.prototype.goToScreen = function (screenId) {
    if (this.screenContainerNode === null) {
        TheFragebogen.logger.error("ScreenController.goToScreen()", "Please call init() before.");
        return false;
    }

    if (!(0 < screenId && screenId < this.screen.length)) {
        TheFragebogen.logger.error("ScreenController.goToScreen()", "The requested screen doesn't exist.");
        return false;
    }

    this.screen[this.currentScreenIndex].releaseUI();
    this.screenContainerNode.innerHTML = null;

    this.currentScreenIndex = screenId;
    this._displayUI();
    return true;
};
/**
 Go to a defined screen immediately. Use event.
 */
ScreenController.prototype._goToScreenEvent = function (event) {
    if (this.screenContainerNode == null) {
        TheFragebogen.logger.error("ScreenController._goToScreenEvent()", "Please call init() before.");
        return false;
    }

    if (event.data != this.currentScreenIndex) {
        if (event.data >= this.screen.length || isNaN(event.data)) {
            TheFragebogen.logger.error("ScreenController._goToScreenEvent()", "The requested screen doesn't exist.");
            return false;
        }

        this.goToScreen(event.data);
    }

    return false;
};
/**
 This is the first class of all the screens that can be used in a questionnaire.<br>
 The screens are where the features of the questionnaire are displayed.<br>
 Only one screen is displayed at once.<br>
 Most of the methods of this class are abstract and might be overridden.
 @abstract
 @class Screen
 */
function Screen() {
}
/**
 Creates the UI of the screen.
 @abstract
 */
Screen.prototype.createUI = function () {
};
/**
 Called when the screen should be shown and can start its job.<br>
 The implementation of this method is optional.
 @abstract
 */
Screen.prototype.start = function () {
};
/**
 Removes all references from and to the screen created by createUI().
 @abstract
 */
Screen.prototype.releaseUI = function () {
};
/**
 Returns the screen data in CSV format.
 @abstract
 */
Screen.prototype.getDataCSV = function () {
};
/**
 TODO: be more specific.?<br>
 Used for localStorage: save.
 @abstract
 */
Screen.prototype.getData = function () {
};
/**
 TODO: be more specific.?<br>
 Used for localStorage: restore.
 @abstract
 */
Screen.prototype._checkData = function (data) {
};
/**
 TODO: documentation
 @abstract
 @param {type} data
 @returns {boolean} 
 */
Screen.prototype.setData = function (data) {
    TheFragebogen.logger.warn("Screen.setData()", "This method should be overriden.");
    return null; 
};
/**
 Checks if the screen is ready, for example if all required questions are
 answered and if the questionnaire can proceed to the next screen.
 @abstract
 @returns {boolean} true if the screen is ready
 */
Screen.prototype.isReady = function () {
    return true;
};

/**
 This is the class of screens that support all the UIElements.<br>
 The questionnaire items are arguments of the screens of this class.<br>
 Only one screen is displayed at once.<br>
 To inherit this class, it should be used ScreenUIElements.apply(this, arguments)
 instead of ScreenUIElements.call(this, arguments). 
 @class ScreenUIElements
 @augments Screen
 @param {string} [className=""] the CSS class that the screen should use
 @param {array} arguments an array containing the UIElements of the screen
 */
function ScreenUIElements(className) {
    Screen.call(this);

    var localArguments = Array.prototype.slice.call(arguments);

    if (typeof (className) !== "string") this.className = "";
    else {
        this.className = className;
        localArguments.splice(0, 1);
    }

    for (var i in localArguments) {
        if (!(localArguments[i] instanceof UIElement))
            TheFragebogen.logger.error("ScreenUIElements():", "This argument is not an UIElement: " + localArguments(i));

    }
    this.elements = localArguments;
    this.node = null;
    this.button = null;
}
ScreenUIElements.prototype = Object.create(Screen.prototype);
ScreenUIElements.prototype.constructor = ScreenUIElements;
/**
 Creates the UI of the screen.
 @returns {object} UI of the ScreenUIElements.
 */
ScreenUIElements.prototype.createUI = function () {
    this.node = document.createElement("div");
    this.node.className = this.className;

    for (var index in this.elements) {
        if (this.elements[index].createUI != undefined) {
            var node = this.elements[index].createUI();
            if (node != null) this.node.appendChild(node);
        } else
            TheFragebogen.logger.warn("ScreenUIElements.createUI():", "Element[" + index + "] has no 'createUI' method");
    }

    this.button = document.createElement("input");
    this.button.type = "button";
    this.button.value = "Next";
    this.button.onclick = function () {
        document.dispatchEvent(new Event("nextScreen"));
    };

    this.node.appendChild(this.button);

    return this.node;
};
/**
 Removes all references from and to the screen created by createUI().
 */
ScreenUIElements.prototype.releaseUI = function () {
    this.node = null;
    this.button = null;
    for (var index in this.elements) {
        this.elements[index].releaseUI();
    }
};
/**
 Called when the screen should be shown and can start its job.<br>
 Enables all the elements of the screen.
 */
ScreenUIElements.prototype.start = function () {
    TheFragebogen.logger.info("ScreenUIElements.start():", "");

    for (var index in this.elements) {
        this.elements[index].setEnabled(true);
    }
};
/**
 Checks if the screen is ready by checking if all its elements are ready.
 @returns {boolean} true if the screen is ready (all elements ready)
 */
ScreenUIElements.prototype.isReady = function () {
    var ready = true;

    for (var index in this.elements) {
        if (this.elements[index] instanceof UIElementInteractive) {
            if (!this.elements[index].isReady()) {
                ready = false;
            }
            this.elements[index].markRequired();
        }
    }
    return ready;
};
/**
 Returns the data of the screen in CSV format.<br>
 The data of each questionnaire item is subdivided in 4 columns:<br>
 1. The type of questionnaire item <br>
 2. The question of the item<br>
 3. The list of answer options, if it is a QuestionnaireItemDefined<br>
 4. The answer of the item
 @returns {array} an array cointaining 4 other arrays, the arrays contain the 
 type, question, list of answer options and answer of each element
 */
ScreenUIElements.prototype.getDataCSV = function () {
    var data = [new Array(), new Array(), new Array(), new Array()];

    for (var index in this.elements) {
        if ((this.elements[index] instanceof QuestionnaireItem) && this.elements[index].isAnswered()) { //TODO Move to QuestionnaireItem AND do this UIElementInteractive
            data[0].push(this.elements[index].getType());
            data[1].push(this.elements[index].getQuestion());
            data[2].push(this.elements[index].getOptionList());
            data[3].push(this.elements[index].getAnswer());
        }
    }

    if (data[0].length == 0) return [];
    return data;
}
/**
 Gets relavant data stored in the element.<br>
 The data depends on the type of the element. <br>
 For a specific element, please look on its documentation. 
 @returns {array} an array that for each element of the screen has another array
 which cointains relevant data of the element
 */
ScreenUIElements.prototype.getData = function () {
    var data = [];

    for (var i in this.elements) {
        data.push(this.elements[i].getData()); //REMINDER these elements should also contain its class
    }

    return data;
};
/**
 TODO: documentation; the method is not clear.
 */
ScreenUIElements.prototype._checkData = function (data) {
    if (!(data instanceof Array) || (data.length != this.elements.length))
        return false;

    for (var i in this.elements) {
        if (!this.elements[i].setData(data[i])) { //TODO Does this make sense?We should only check!
            TheFragebogen.logger.warn("ScreenUIElements._checkData()", "Provided data is not valid for element " + i + ".");
            return false;
        }
    }
    return true;
};
/**
 TODO: documentation; depends on the _checkData() method, which is not clear.
 */
ScreenUIElements.prototype.setData = function (data) {
    if (!this._checkData(data)) {
        TheFragebogen.logger.warn("ScreenUIElements.setData()", "Provided data is not valid.");
        return false;
    }

    for (var i in this.elements)
        this.elements[i].setData(data[i]);
    return true;
};

/**
 This class of screens supports all the UIElements.<br>
 The questionnaire items are arguments of these screens.<br>
 Only one screen is displayed at once.<br>
 The UIElementsInteractive are enabled sequentially on this class of screen.<br>
 Each element is only enabled after the previous one has been answered.
 @class ScreenUIElementsSequential
 @augments Screen
 @augments ScreenUIElements
 @param {string} [className=""] the CSS class that the screen should use
 @param {array} arguments an array containing the UIElements of the screen
 */
//TODO This screen is not working properly:
//1. When the first element of the screen is not interactive, the other elements are never enabled
//2. When there is a QuestionnaireItemWrite on the screen, every time the mouses passes through the element a new element is enabled
//TODO Implement getData, _checkData and setData to reflect currentIndex.
function ScreenUIElementsSequential(className) {
    ScreenUIElements.apply(this, arguments);
    this.currentElementIndex = null;
}
ScreenUIElementsSequential.prototype = Object.create(ScreenUIElements.prototype);
ScreenUIElementsSequential.prototype.constructor = ScreenUIElementsSequential;
/**
 Called when the screen should be shown and can start its job.<br>
 Enables only the first element of the screen.
 */
ScreenUIElementsSequential.prototype.start = function () {
    for (var index in this.elements) {
        if (typeof this.elements[index].setOnReadyCallback === "function") {
            this.elements[index].setOnReadyCallback((this.onReadyCallback).bind(this));
        }
        this.elements[index].setEnabled(false);
    }

    this.currentElementIndex = 0;
    this.elements[this.currentElementIndex].setEnabled(true);
}
/**
 Enables the next interactive element, every time the last enabled element is 
 answered. 
 */
ScreenUIElementsSequential.prototype.onReadyCallback = function () {
    TheFragebogen.logger.info("ScreenUIElementsSequential.onReadyCallback()", "This method might be overriden.");

    var i = -1;
    for (i = this.currentElementIndex + 1; i < this.elements.length; i++) {
        this.elements[i].setEnabled(true);
        if (typeof this.elements[i].setOnReadyCallback === "function") {
            break;
        }
    }
    this.currentElementIndex = i;
}

/**
 This class of screens displays a message for some seconds.<br>
 After the timeout, the questionnaire goes to the next screen.<br>
 Only one screen is displayed at once.<br>
 Some of the methods of this class are abstract.
 @class ScreenWait
 @augments Screen
 @param {string} [className=""] the CSS class that the screen should use
 @param {number} [time=0] timeout of the screen in seconds
 @param {string} [message="Please wait..."] message that will be displayed on the screen
 */
function ScreenWait(className, time, html) {
    Screen.call(this);

    this.className = (typeof (className) !== "string") ? "" : className;
    this.time = !isNaN(time) ? Math.abs(time) * 1000 : 0;
    this.html = typeof html === "string" ? html : "Please wait...";
    this.state = null; 
    this.timeoutHandle;

    TheFragebogen.logger.debug("ScreenWait()", "Set: time as " + this.time + " and html as " + this.html);
}
ScreenWait.prototype = Object.create(Screen.prototype);
ScreenWait.prototype.constructor = ScreenWait;
/**
 Creates the UI of the screen.
 @returns {object} UI of the ScreenWait
 */
ScreenWait.prototype.createUI = function () {
    this.node = document.createElement("div");
    this.node.className = this.className;

    var span = document.createElement("span");
    span.innerHTML = this.html;
    this.node.appendChild(span);

    this.state = "created";

    return this.node;
};
/**
Returns the current state of the screen.
 */
ScreenWait.prototype.getState = function(){
    return this.state;
};
/**
 Sets the timeout of the screen.
 */
ScreenWait.prototype._startTimer = function () {
    TheFragebogen.logger.info("ScreenWait._startTimer()", "New screen will be displayed in " + this.time + "ms.");
    this.timeoutHandle = setTimeout((this._handleTimeout).bind(this), this.time);
    this.state = "running";
};
/**
 Called when the screen should be shown and can start its job.<br>
 The method _startTimer() is called.
 */
ScreenWait.prototype.start = function () {
    this._startTimer();
}
/**
 Goes to the next screen 
 */
ScreenWait.prototype._handleTimeout = function () {
    this.state = "finished";
    document.dispatchEvent(new Event("nextScreen"));
};
/**
 Removes all references from and to the screen created by createUI().
 */
ScreenWait.prototype.releaseUI = function () {
    clearTimeout(this.timeoutHandle);
    this.timeoutHandle = null;
    this.state = null;
};

/**
 A screen showing a full-screen iframe.<br>
 This screen sends a _nextScreen_-Event, when the location of the iframe is changed (by the iframe itself).<br>
 Returns the time between: ScreenIFrame.start() and the location change of the IFrame.<br>
 !!!Attention: This might be misleading depending on your timing requirements!
 @class ScreenIFrame
 @augments Screen
 @param {string} url The URL to be opened in the iframe.
 */
function ScreenIFrame(url) {
    Screen.call(this);

    this.url = url;

    this.iframe = null;
    this.startTime = null;

    this.time = null;
    this.returnURL = null;
}
ScreenIFrame.prototype = Object.create(Screen.prototype);
ScreenIFrame.prototype.constructor = ScreenIFrame;
ScreenIFrame.prototype.createUI = function () {
    this.iframe = document.createElement("iframe");
    this.iframe.src = this.url;
    this.iframe.onload = function (event) {
        event.srcElement.onload = function (event) { //Replace the onload-handler after the page is loaded, because the following the next redirect.
            this.time = Date.now() - this.startTime;
//			console.log(event.target.contentWindow.location.href); //The new URL of the iframe can be used to pass information back to TheFragebogen.
            document.dispatchEvent(new Event("nextScreen"));
        }.bind(this);
    }.bind(this);

    this.iframe.frameBorder = 0;
    this.iframe.height = "100%";
    this.iframe.width = "100%";

    return this.iframe;
};
ScreenIFrame.prototype.start = function () {
    this.startTime = Date.now();
};
ScreenIFrame.prototype.isReady = function () {
    return this.time !== null;
};
ScreenIFrame.prototype.releaseUI = function () {
    this.iframe = null;
    this.startTime = null;
    TheFragebogen.logger.info("ScreenIFrame.releaseUI()", this.time);
};
ScreenIFrame.prototype.getDataCSV = function () {
    return [["time", "returnURL"], [this.time, this.returnURL]];
}
ScreenIFrame.prototype.getData = function () {
    return [this.time, this.returnURL];
}
ScreenIFrame.prototype._checkData = function (data) {
    if (data[0] == null && data[1] == null) return true;
    if (typeof data[0] === "number" && data[0] > 0 && typeof data[1] === "string")
        return true;
    return false;
}
ScreenIFrame.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;
    this.time = data[0];
    this.returnURL = data[1];
}
/**
 Uploads the current data of the questionnaire in CSV format to a webserver via AJAX.<br>
 Be aware of Cross-site origin: http://en.wikipedia.org/wiki/Cross-origin_resource_sharing<br>
 <br>
 Uses HTTP POST.<br>
 <br>
 Displays a message while the data is uploaded.<br>
 After the upload or the timeout, the questionnaire goes to the next screen.<br>
 Only one screen is displayed at once.
 @class ScreenWaitDataUpload
 @augments Screen
 @augments ScreenWait
 @param {string} [className=""] the CSS class that the screen should use
 @param {string} url where the data should be uploaded
 @param {number} [timeout=4] timeout of the screen in seconds
 @param {string} [httpParamaterName="data"] name of the parameters that will be send to the url
 @param {string} [message="Uploading data. Please wait..."] message that is displayed on the screen
 */
//TODO Request failed case is not implemented.
//TODO Add error message if URL undefined / unparseable.
//TODO Handle retries
//TODO Provide user feedback
//TODO Provide minimal time to show. 
function ScreenWaitDataUpload(className, url, timeout, httpParameterName, message) {
    ScreenWait.call(this, className, !isNaN(timeout) ? Math.abs(timeout) * 1 : 4, typeof message === "string" ? message : "Uploading data. Please wait...");

    this.httpParameterName = (typeof httpParameterName === "string" ? httpParameterName : "data");
    this.url = url;
    this.request = null;

    TheFragebogen.logger.debug("ScreenDataUpload()", "Set: httpParameterName as " + this.httpParameterName);
}
ScreenWaitDataUpload.prototype = Object.create(Screen.prototype);
ScreenWaitDataUpload.prototype.constructor = ScreenWaitDataUpload;
/**
 Creates the UI of the screen.
 @returns {object} UI of the ScreenWaitUpload
 */
ScreenWaitDataUpload.prototype.createUI = function () {
    this.node = document.createElement("div");

    var span = document.createElement("span");
    span.innerHTML = this.html;
    this.node.appendChild(span);

    return this.node;
};
/**
 Called when the screen should be shown and can start its job.<br>
 All the data of the questionnaire already stored is requested in CSV format.<br>
 The event requestDataCSV and the method callbackUpload() are called.
 */
ScreenWaitDataUpload.prototype.start = function () {
    var event = new Event("requestDataCSV");
    event.callback = (this.callbackUpload).bind(this);
    document.dispatchEvent(event);
}
/**
 Tries to upload in CSV format all the data of the questionnaire.<br>
 @param {object} data result of the event requestDataCSV
 */
ScreenWaitDataUpload.prototype.callbackUpload = function (data) {
    TheFragebogen.logger.info("ScreenWaitDataUpload.callbackUpload()", "Starting upload to " + this.url);

    this.request = new XMLHttpRequest();
    this.request.open("POST", this.url, true);
    this.request.timeout = this.time;

    this.request.ontimeout = (this._ontimeout).bind(this);
    this.request.onload = (this._onload).bind(this);
    this.request.onerror = (this._onerror).bind(this);

    this.request.send(this.httpParameterName + "=" + data);
};
/**
 Confirms if the upload was successful and goes to the next screen.
 */
ScreenWaitDataUpload.prototype._onload = function () {
    if (this.request.readyState === 4 && this.request.status === 200) {
        TheFragebogen.logger.info("ScreenWaitDataUpload.callbackUpload()", "Successful.");
        if (this.request.responseText != "")
            TheFragebogen.logger.info("ScreenWaitDataUpload._onload()", this.request.responseText);
        document.dispatchEvent(new Event("nextScreen"));
    }
    this.request = null;
}
/**
 Reports that the upload has failed.<br>
 Logs the request status.
 */
ScreenWaitDataUpload.prototype._onerror = function () {
    TheFragebogen.logger.error("ScreenWaitDataUpload.callbackUpload()", "Upload failed with HTTP code: " + this.request.status);
    this.request = null;
}
/**
 Reports that the upload was not completed under the maximum time.
 */
ScreenWaitDataUpload.prototype._ontimeout = function () {
    TheFragebogen.logger.error("ScreenWaitDataUpload.callbackUpload()", "Upload got timeout after " + this.time + "ms.");
    this.request = null;
}
/**
 Removes all references from and to the screen created by createUI().
 */
ScreenWaitDataUpload.prototype.releaseUI = function () {
    this.node = null;

    if (this.request instanceof XMLHttpRequest) this.request.abort();
    this.request = null;
};


/**
 Triggers the download of the current data of the questionnaire in csv format.
 Displays a message while the data is downloaded.<br>
 After the timeout, the questionnaire goes to the next screen.<br>
 Only one screen is displayed at once.
 
 <br>
 TODO the timeout need to be changed it is 5min now.
 TODO Add re-scheduling of timer, if not work is not yet ready (should go into a super-class).
 @class ScreenWaitDataDownload
 @augments Screen
 @augments ScreenWait
 @param {string} [className=""] the CSS class that the screen should use
 @param {string} [message="Downloading data"] message that is displayed on the screen
 @param {string} [filename="Answers.csv"] name of the file that will be downloaded
 */
function ScreenWaitDataDownload(className, message, filename) {
    ScreenWait.call(this, className === "string" ? className : "Downloading data", 300, typeof message === "string" ? message : "Downloading data");

    this.filename = (typeof filename === "string" ? filename : "Answers.csv");

    TheFragebogen.logger.debug("ScreenDataDownload()", "Set: filename as " + this.filename);

}
ScreenWaitDataDownload.prototype = Object.create(ScreenWait.prototype);
ScreenWaitDataDownload.prototype.constructor = ScreenWaitDataDownload;
/**
 Creates the UI of the screen.
 @returns {object} UI of the ScreenWaitDownload
 */
ScreenWaitDataDownload.prototype.createUI = function () {
    this.node = document.createElement("div");

    var span = document.createElement("span");
    span.innerHTML = this.html;
    this.node.appendChild(span);

    return this.node;
};
/**
 Called when the screen should be shown and can start its job.<br>
 All the data of the questionnaire already stored is requested in CSV format.<br>
 The event requestDataCSV and the method callbackDownload() are called.
 */
ScreenWaitDataDownload.prototype.start = function () {
    var event = new Event("requestDataCSV");
    event.callback = (this.callbackDownload).bind(this);
    document.dispatchEvent(event);
};
/**
 Downloads in CSV format all the data of the questionnaire.<br>
 Calls the function _startTimer() that sets the timeout (5 min) of the screen.
 @param {object} data result of the event requestDataCSV
 */
ScreenWaitDataDownload.prototype.callbackDownload = function (data) {
    TheFragebogen.logger.info("ScreenWaitDataDownload.callbackDownload()", data);
    if ('download' in document.createElement('a') && navigator.userAgent.toLowerCase().
            indexOf('firefox') == -1) { //So far only chrome AND not firefox.
        var downloadLink = document.createElement("a");
        downloadLink.download = this.filename;
        downloadLink.href = window.URL.createObjectURL(new Blob([data
        ], {type: 'text/plain'}));
        downloadLink.click();
        window.URL.revokeObjectURL(downloadLink.href); //Release object: https://developer.mozilla.org/en-US/docs/Web/API/URL.revokeObjectURL
        this._startTimer();
        return;
    }
    window.location.href = "data:application/x-download;charset=utf-8," + encodeURIComponent(data);
};/**
 A element provides an interactive UI with media.<br>
 @abstract
 @class UIElementInteractiveMedia
 @augments UIElementInteractive
 @param {string} url
 @param {boolen} required
 */
function UIElementInteractiveMedia(url, required) {
    UIElementInteractive.call(this);

    this.url = url;
    this.required = required;
    this.loaded = false;

    TheFragebogen.logger.debug("UIElementInteractiveMedia()", "Set: url as " + this.url + " and required as " + this.required);

}
UIElementInteractiveMedia.prototype = Object.create(UIElementInteractive.prototype);
UIElementInteractiveMedia.prototype.constructor = UIElementInteractiveMedia;
UIElementInteractiveMedia.prototype.isMediaLoaded = function () {
    return this.loaded;
};
UIElementInteractiveMedia.prototype.isReady = function () {
    return this.isRequired() ? this.played : true;
};
UIElementInteractiveMedia.prototype.isRequired = function () {
    return this.required;
};

/**
 Plays an audio-file showing a progressbar.<br>
 Reports playback finished via ready-callback.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <audio></audio>
 <progress></progress>
 </div>
 @class UIElementInteractiveMediaAudio
 @augments UIElementInteractive
 @param {string} [className] CSS-style
 @param {url} Url to load.
 @param {boolean} replayable if the audio can be replayed
 @param {boolean} [required=false] if it is required to be played
 */
function UIElementInteractiveMediaAudio(className, url, replayable, required) {
    UIElementInteractiveMedia.call(this, url, required);

    this.node = null;

    this.className = (typeof className === "string" ? className : "");
    this.replayable = (typeof replayable === "boolean" ? replayable : false);
    this.played = false;
    this.audio = null;
    this.progress = null;

    TheFragebogen.logger.debug("UIElementInteractiveMediaAudio()", "Set: className as " + this.className + " and replayable as " + this.replayable);

    this.audio = new Audio(this.url);
    this.audio.addEventListener("canplaythrough", function () {
        this.loaded = true;
    }.bind(this), true);
}
UIElementInteractiveMediaAudio.prototype = Object.create(UIElementInteractiveMedia.prototype);
UIElementInteractiveMediaAudio.prototype.constructor = UIElementInteractiveMediaAudio;
UIElementInteractiveMediaAudio.prototype.createUI = function () {
    if (this.isUIcreated())
        return;

    this.node = document.createElement("div");
    this.node.className = this.className;

    this.uiCreated = true;

    this.audio.load();//TODO: This is an workaround for Android, because media cannot be loaded automatically but require user input.

    this.audio.ontimeupdate = (this._timeUpdate).bind(this);
    this.audio.onended = (this._ended).bind(this);
    this.node.appendChild(this.audio);

    this.progress = document.createElement("progress");
    this.progress.onclick = (this._replay).bind(this);
    this.node.appendChild(this.progress);

    return this.node;
};
/**
 Enabling starts playback.
 */
UIElementInteractiveMediaAudio.prototype.setEnabled = function (enabled) {
    this.enabled = this.isUIcreated() ? enabled : false;
    if (this.enabled && this.audio != null)
        this.audio.play();
}
UIElementInteractiveMediaAudio.prototype.markRequired = function () {
    if (this.node == null)
        return;

    if (!this.isReady())
        this.node.className = "UIElementInteractiveRequired";
    else
        this.node.className = null;
}
UIElementInteractiveMediaAudio.prototype._replay = function () {
    if (this.played && this.replayable && this.isEnabled()) {
        this.audio.currentTime = 0;
        this.audio.play();
    }
};
UIElementInteractiveMediaAudio.prototype._timeUpdate = function () {
    if (this.progress && !isNaN(this.audio.duration)) {
//		console.log("", "Time update " + this.audio.currentTime + " / " + this.audio.duration); // change to TheFragebogen.logger.something and where?
        this.progress.value = (this.audio.currentTime / this.audio.duration);
    }
};
UIElementInteractiveMediaAudio.prototype._ended = function () {
    this.played = true;
    this._sendReadyCallback();
    this.markRequired();
};
UIElementInteractiveMediaAudio.prototype._error = function () {
    TheFragebogen.logger.error("UIElementInteractiveMediaAudio._error", "An error occured.");
    alert("The audio-file " + this.url + " could not be found.");
};
UIElementInteractiveMediaAudio.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.played = false;
    if (this.audio instanceof Audio)
        this.audio.pause();
    this.audio = null;
    this.progress = null;
};
UIElementInteractiveMediaAudio.prototype.getData = function () {
    return [this.url, this.replayable];
};
UIElementInteractiveMediaAudio.prototype._checkData = function (data) {
    return (data[0] == this.url) && (data[1] == this.replayable);
};
UIElementInteractiveMediaAudio.prototype.setData = function (data) {
    return this._checkData(data);
};

/**
 Plays an video-file.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <video></video>
 </div>
 @class UIElementInteractiveMediaVideo
 @augments UIElementInteractiveMedia
 @param {string} [className] CSS-style
 @param {string} url URL
 @param {number} [height] video height
 @param {number} [width] video width
 @param {boolean} [controls] controls of the html5 video element
 @param {boolean} [required=false] if it is required to be played
 */
function UIElementInteractiveMediaVideo(className, url, height, width, controls, required) {
    UIElementInteractiveMedia.call(this, url, required);

    this.node = null;

    this.played = false;
    this.className = (typeof className === "string" ? className : "");
    this.height = !isNaN(height) ? height : 320;
    this.width = !isNaN(width) ? width : 480;
    this.controls = (typeof controls === "boolean" ? controls : true);

    TheFragebogen.logger.debug("UIElementInteractiveMediaVideo()", "Set: className as " + this.className + ", height as " + this.height + ", width as " + this.width + " and controls as " + this.controls);

    this.node = document.createElement('video');
    this.node.src = this.url;
    this.node.addEventListener("canplaythrough", function () {
        this.loaded = true;
    }.bind(this), true);
}
UIElementInteractiveMediaVideo.prototype = Object.create(UIElementInteractiveMedia.prototype);
UIElementInteractiveMediaVideo.prototype.constructor = UIElementInteractiveMediaVideo;
UIElementInteractiveMediaVideo.prototype.createUI = function () {
    if (this.isUIcreated())
        return;

    this.node.className = this.className;

    this.uiCreated = true;

    this.node.load();//TODO: This is an workaround for Android, because media cannot be loaded automatically but require user input.

    this.node.height = this.height;
    this.node.width = this.width;
    this.node.autobuffer = true;
    this.node.controls = this.controls;
    this.node.autoplay = !this.controls;
    this.node.onended = (this._ended).bind(this);

    return this.node;
};
/**
 Enabling starts playback.
 */
UIElementInteractiveMediaVideo.prototype.setEnabled = function (enabled) {
    this.enabled = this.isUIcreated() ? enabled : false;
    if (this.enabled && this.video != null)
        this.node.play();
};
UIElementInteractiveMediaVideo.prototype.markRequired = function () {
    if (this.node == null)
        return;

    if (!this.isReady())
        this.node.className = "UIElementInteractiveRequired";
    else
        this.node.className = null;
};
UIElementInteractiveMediaVideo.prototype._ended = function () {
    this.played = true;
    this._sendReadyCallback();
    this.markRequired();
};
UIElementInteractiveMediaVideo.prototype._error = function () {
    TheFragebogen.logger.error("UIElementInteractiveMediaVideo._error", "An error occured.");
    alert("The video-file " + this.url + " could not be found.");
};
UIElementInteractiveMediaVideo.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.played = false;
    if (this.node != null)
        this.node.pause();
    this.node = null;
};
UIElementInteractiveMediaVideo.prototype.getData = function () {
    return [this.url, this.controls, this.required];
};
UIElementInteractiveMediaVideo.prototype._checkData = function (data) {
    return (data[0] == this.url) && (data[1] == this.controls) && (data[2] == this.required);
};
UIElementInteractiveMediaVideo.prototype.setData = function (data) {
    return this._checkData(data);
};

/**
 Display an image for a defined or undefined amount of time.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <div>
 <img>
 </div>
 @class UIElementHTMLImage
 @augments UIElementHTML
 @param {string} [className] CSS-style
 @param {number} [height] image height
 @param {number} [width] image width
 @param {number} [time=0] for how long the image will be displayed (when 0, it will be forever displayed)
 */
function UIElementHTMLImage(className, url, height, width, time) {
    UIElementHTML.call(this);

    this.className = (typeof className === "string" ? className : "");
    this.height = !isNaN(height) ? height : 320;
    this.width = !isNaN(width) ? width : 480;
    this.time = !isNaN(time) ? time : 0;
    this.node = null;
    this.url = url;

    TheFragebogen.logger.debug("UIElementHTMLImage()", "Set: className as " + this.className + ", height as " + this.height + ", width as " + this.width + " and time as " + this.time);

    this.loaded = false;
    this.img = new Image(this.width, this.height);
    this.img.src = this.url;
    this.img.onload = function () {
        this.loaded = true;
    };
}
UIElementHTMLImage.prototype = Object.create(UIElementHTML.prototype);
UIElementHTMLImage.prototype.constructor = UIElementHTMLImage;
UIElementHTMLImage.prototype.createUI = function () {
    if (this.isUIcreated())
        return;

    this.node = document.createElement("div");
    this.node.className = this.className;
    this.node.appendChild(this.img);

    this.uiCreated = true;

    if (this.time != 0)
        this.timeoutHandle = setTimeout(function () {
            this.img.style.visibility = "hidden";
        }.bind(this), this.time);

    return this.node;
};
UIElementHTMLImage.prototype.setEnabled = function (enabled) {
    this.enabled = this.isUIcreated() ? enabled : false;
};
UIElementHTMLImage.prototype.markRequired = function () {
    if (this.node == null)
        return;
    else
        this.node.className = null;
};
UIElementHTMLImage.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    clearTimeout(this.timeoutHandle);
    this.timeoutHandle = null;

    this.img = null;
};
UIElementHTMLImage.prototype.getData = function () {
    return [this.url, this.time];
};
UIElementHTMLImage.prototype._checkData = function (data) {
    return (data[0] == this.url) && (data[1] == this.time);
};
UIElementHTMLImage.prototype.setData = function (data) {
    return this._checkData(data);
};
UIElementHTMLImage.prototype.isMediaLoaded = function () {
    return this.loaded;
};

/**
 A screen playing a video in full-screen.<br> 
 After playing the video, this screen proceeds automatically.
 
 Is fullscreen with a black background. CSS styling not available.
 @example
 Basic HTML Output:
 <div>
    <video>
    </video>
 </div>
 @class ScreenMediaVideo
 @augments Screen
 @param url The URL of the video.
 */

//TODO Add proper preloading
function ScreenMediaVideo(url) {
    Screen.call(this);
    this.url = url;
    this.played = false;
}
ScreenMediaVideo.prototype = Object.create(Screen.prototype);
ScreenMediaVideo.prototype.constructor = ScreenMediaVideo;
ScreenMediaVideo.prototype.createUI = function () {
    this.node = document.createElement('video');
    this.node.style.backgroundColor = "black";
    this.node.style.position = "absolute";
    this.node.style.left = "0px";
    this.node.style.top = "0px";
    this.node.style.height = "100%";
    this.node.style.width = "100%";

    this.node.poster = "/"; //Do not use a default poster; otherwise it flickers on Android.
    this.node.onended = (this._ended).bind(this);
    this.node.onplay = (this._play).bind(this)

    this.node.src = this.url;

    this.node.load();

    return this.node;
};
ScreenMediaVideo.prototype.start = function () {
   this.node.play();
};
/**
 Callback for video playback started.
 */
ScreenMediaVideo.prototype._play = function () {
    this.node.style.visibility = "visible";
};
/**
 Callback for video playback finished.
 */
ScreenMediaVideo.prototype._ended = function () {
    this.played = true;
    document.dispatchEvent(new Event("nextScreen"));
};
ScreenMediaVideo.prototype.isReady = function () {
    return this.played;
};
ScreenMediaVideo.prototype.releaseUI = function () {
    this.uiCreated = false;

    if (this.node != null) this.node.pause();
    this.node = null;
};
ScreenMediaVideo.prototype.getData = function () {
    return [this.url];
};
ScreenMediaVideo.prototype._checkData = function (data) {
    return (data[0] == this.url);
};
ScreenMediaVideo.prototype.setData = function (data) {
    return this._checkData(data);
};
// MIT License:
//
// Copyright (c) 2010-2012, Joe Walnes
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/**
 * This behaves like a WebSocket in every way, except if it fails to connect,
 * or it gets disconnected, it will repeatedly poll until it successfully connects
 * again.
 *
 * It is API compatible, so when you have:
 *   ws = new WebSocket('ws://....');
 * you can replace with:
 *   ws = new ReconnectingWebSocket('ws://....');
 *
 * The event stream will typically look like:
 *  onconnecting
 *  onopen
 *  onmessage
 *  onmessage
 *  onclose // lost connection
 *  onconnecting
 *  onopen  // sometime later...
 *  onmessage
 *  onmessage
 *  etc...
 *
 * It is API compatible with the standard WebSocket API, apart from the following members:
 *
 * - `bufferedAmount`
 * - `extensions`
 * - `binaryType`
 *
 * Latest version: https://github.com/joewalnes/reconnecting-websocket/
 * - Joe Walnes
 *
 * Syntax
 * ======
 * var socket = new ReconnectingWebSocket(url, protocols, options);
 *
 * Parameters
 * ==========
 * url - The url you are connecting to.
 * protocols - Optional string or array of protocols.
 * options - See below
 *
 * Options
 * =======
 * Options can either be passed upon instantiation or set after instantiation:
 *
 * var socket = new ReconnectingWebSocket(url, null, { debug: true, reconnectInterval: 4000 });
 *
 * or
 *
 * var socket = new ReconnectingWebSocket(url);
 * socket.debug = true;
 * socket.reconnectInterval = 4000;
 *
 * debug
 * - Whether this instance should log debug messages. Accepts true or false. Default: false.
 *
 * reconnectInterval
 * - The number of milliseconds to delay before attempting to reconnect. Accepts integer. Default: 1000.
 *
 * reconnectDecay
 * - The rate of increase of the reconnect delay. Allows reconnect attempts to back off when problems persist. Accepts integer or float. Default: 1.5.
 *
 * timeoutInterval
 * - The maximum time in milliseconds to wait for a connection to succeed before closing and retrying. Accepts integer. Default: 2000.
 *
 */
(function (global, factory) {
    if (typeof define === 'function' && define.amd) {
        define([], factory);
    } else if (typeof module !== 'undefined' && module.exports) {
        module.exports = factory();
    } else {
        global.ReconnectingWebSocket = factory();
    }
})(this, function () {

    function ReconnectingWebSocket(url, protocols, options) {

        // Default settings
        var settings = {
            /** Whether this instance should log debug messages. */
            debug: false,
            /** The number of milliseconds to delay before attempting to reconnect. */
            reconnectInterval: 1000,
            /** The rate of increase of the reconnect delay. Allows reconnect attempts to back off when problems persist. */
            reconnectDecay: 1.5,
            /** The maximum time in milliseconds to wait for a connection to succeed before closing and retrying. */
            timeoutInterval: 2000
        }
        if (!options) {
            options = {};
        }

        // Overwrite and define settings with options if they exist.
        for (var key in settings) {
            if (typeof options[key] !== 'undefined') {
                this[key] = options[key];
            } else {
                this[key] = settings[key];
            }
        }

        // These should be treated as read-only properties

        /** The URL as resolved by the constructor. This is always an absolute URL. Read only. */
        this.url = url;
        /** The number of attempted reconnects since starting, or the last successful connection. Read only. */
        this.reconnectAttempts = 0;
        /**
         * The current state of the connection.
         * Can be one of: WebSocket.CONNECTING, WebSocket.OPEN, WebSocket.CLOSING, WebSocket.CLOSED
         * Read only.
         */
        this.readyState = WebSocket.CONNECTING;
        /**
         * A string indicating the name of the sub-protocol the server selected; this will be one of
         * the strings specified in the protocols parameter when creating the WebSocket object.
         * Read only.
         */
        this.protocol = null;

        // Private state variables

        var self = this;
        var ws;
        var forcedClose = false;
        var timedOut = false;
        var eventTarget = document.createElement('div');

        // Wire up "on*" properties as event handlers

        eventTarget.addEventListener('open', function (event) {
            self.onopen(event);
        });
        eventTarget.addEventListener('close', function (event) {
            self.onclose(event);
        });
        eventTarget.addEventListener('connecting', function (event) {
            self.onconnecting(event);
        });
        eventTarget.addEventListener('message', function (event) {
            self.onmessage(event);
        });
        eventTarget.addEventListener('error', function (event) {
            self.onerror(event);
        });

        // Expose the API required by EventTarget

        this.addEventListener = eventTarget.addEventListener.bind(eventTarget);
        this.removeEventListener = eventTarget.removeEventListener.bind(eventTarget);
        this.dispatchEvent = eventTarget.dispatchEvent.bind(eventTarget);

        /**
         * This function generates an event that is compatible with standard
         * compliant browsers and IE9 - IE11
         * 
         * This will prevent the error:
         * Object doesn't support this action
         * 
         * http://stackoverflow.com/questions/19345392/why-arent-my-parameters-getting-passed-through-to-a-dispatched-event/19345563#19345563
         * @param s String The name that the event should use
         * @param args Object an optional object that the event will use
         */
        function generateEvent(s, args) {
            var evt = document.createEvent("CustomEvent");
            evt.initCustomEvent(s, false, false, args);
            return evt;
        }
        ;

        function connect(reconnectAttempt) {
            ws = new WebSocket(url, protocols || []);

            if (!reconnectAttempt) {
                eventTarget.dispatchEvent(generateEvent('connecting'));
            }

            if (self.debug || ReconnectingWebSocket.debugAll) {
                TheFragebogen.logger.debug('ReconnectingWebSocket', 'attempt-connect', url);
            }

            var localWs = ws;
            var timeout = setTimeout(function () {
                if (self.debug || ReconnectingWebSocket.debugAll) {
                    TheFragebogen.logger.debug('ReconnectingWebSocket', 'connection-timeout', url);
                }
                timedOut = true;
                localWs.close();
                timedOut = false;
            }, self.timeoutInterval);

            ws.onopen = function (event) {
                clearTimeout(timeout);
                if (self.debug || ReconnectingWebSocket.debugAll) {
                    TheFragebogen.logger.debug('ReconnectingWebSocket', 'onopen', url);
                }
                self.protocol = ws.protocol;
                self.readyState = WebSocket.OPEN;
                self.reconnectAttempts = 0;
                var e = generateEvent('open');
                e.isReconnect = reconnectAttempt;
                reconnectAttempt = false;
                eventTarget.dispatchEvent(e);
            };

            ws.onclose = function (event) {
                clearTimeout(timeout);
                ws = null;
                if (forcedClose) {
                    self.readyState = WebSocket.CLOSED;
                    eventTarget.dispatchEvent(generateEvent('close'));
                } else {
                    self.readyState = WebSocket.CONNECTING;
                    var e = generateEvent('connecting');
                    e.code = event.code;
                    e.reason = event.reason;
                    e.wasClean = event.wasClean;
                    eventTarget.dispatchEvent(e);
                    if (!reconnectAttempt && !timedOut) {
                        if (self.debug || ReconnectingWebSocket.debugAll) {
                            TheFragebogen.logger.debug('ReconnectingWebSocket', 'onclose', url);
                        }
                        eventTarget.dispatchEvent(generateEvent('close'));
                    }
                    setTimeout(function () {
                        self.reconnectAttempts++;
                        connect(true);
                    }, self.reconnectInterval * Math.pow(self.reconnectDecay, self.reconnectAttempts));
                }
            };
            ws.onmessage = function (event) {
                if (self.debug || ReconnectingWebSocket.debugAll) {
                    TheFragebogen.logger.debug('ReconnectingWebSocket', 'onmessage', url, event.data);
                }
                var e = generateEvent('message');
                e.data = event.data;
                eventTarget.dispatchEvent(e);
            };
            ws.onerror = function (event) {
                if (self.debug || ReconnectingWebSocket.debugAll) {
                    TheFragebogen.logger.debug('ReconnectingWebSocket', 'onerror', url, event);
                }
                eventTarget.dispatchEvent(generateEvent('error'));
            };
        }
        connect(false);

        /**
         * Transmits data to the server over the WebSocket connection.
         *
         * @param data a text string, ArrayBuffer or Blob to send to the server.
         */
        this.send = function (data) {
            if (ws) {
                if (self.debug || ReconnectingWebSocket.debugAll) {
                    TheFragebogen.logger.debug('ReconnectingWebSocket', 'send', url, data);
                }
                return ws.send(data);
            } else {
                throw 'INVALID_STATE_ERR : Pausing to reconnect websocket';
            }
        };

        /**
         * Closes the WebSocket connection or connection attempt, if any.
         * If the connection is already CLOSED, this method does nothing.
         */
        this.close = function (code, reason) {
            // Default CLOSE_NORMAL code
            if (typeof code == 'undefined') {
                code = 1000;
            }
            forcedClose = true;
            if (ws) {
                ws.close(code, reason);
            }
        };

        /**
         * Additional public API method to refresh the connection if still open (close, re-open).
         * For example, if the app suspects bad data / missed heart beats, it can try to refresh.
         */
        this.refresh = function () {
            if (ws) {
                ws.close();
            }
        };
    }

    /**
     * An event listener to be called when the WebSocket connection's readyState changes to OPEN;
     * this indicates that the connection is ready to send and receive data.
     */
    ReconnectingWebSocket.prototype.onopen = function (event) {
    };
    /** An event listener to be called when the WebSocket connection's readyState changes to CLOSED. */
    ReconnectingWebSocket.prototype.onclose = function (event) {
    };
    /** An event listener to be called when a connection begins being attempted. */
    ReconnectingWebSocket.prototype.onconnecting = function (event) {
    };
    /** An event listener to be called when a message is received from the server. */
    ReconnectingWebSocket.prototype.onmessage = function (event) {
    };
    /** An event listener to be called when an error occurs. */
    ReconnectingWebSocket.prototype.onerror = function (event) {
    };

    /**
     * Whether all instances of ReconnectingWebSocket should log debug messages.
     * Setting this to true is the equivalent of setting all instances of ReconnectingWebSocket.debug to true.
     */
    ReconnectingWebSocket.debugAll = false;

    ReconnectingWebSocket.CONNECTING = WebSocket.CONNECTING;
    ReconnectingWebSocket.OPEN = WebSocket.OPEN;
    ReconnectingWebSocket.CLOSING = WebSocket.CLOSING;
    ReconnectingWebSocket.CLOSED = WebSocket.CLOSED;

    return ReconnectingWebSocket;
});
//TODO Refactor both classes; avoid using the status property with a string.
//TODO Combine with QuestionnaireItemWaitWebsocket.
//TODO Use css class for feedback icon!
/**
 A screen that connects to a websocket-server and waits for a signal before continue.<br>
 This screen sends a _nextScreen_-Event, when a signal via websocket is received OR on timeout.
 @class ScreenWaitWebsocket
 @augments ScreenWait
 @param url The Websocket-URL to connect to.
 @param [msg=""] The message that should be received to dispatch event for next screen.
 @param [timeout=0] Automatically proceed, if the timeout is reached.

 */

function ScreenWaitWebsocket(className, url, msg, timeout) {

    ScreenWait.call(this, className);

    this.url = url || "ws://localhost:8080";
    this.msg = typeof msg === "string" ? msg : "";
    this.timeout = !isNaN(timeout) ? Math.abs(timeout) * 1000 : 0;
    this.timeoutHandle;

    this.status = null;

    this.node = null;
    this.statusBubble = null;
    this.connection = null;
    this.attempts = 0;

    TheFragebogen.logger.info("ScreenWaitWebsocket()", "Set: url as " + this.url + ", message as " + this.msg + " and timeout as " + this.timeout);
}
ScreenWaitWebsocket.prototype = Object.create(ScreenWait.prototype);
ScreenWaitWebsocket.prototype.start = function () {
    TheFragebogen.logger.info("ScreenWaitWebsocket.start()", "Set: url as " + this.url + ", message as " + this.msg + " and timeout as " + this.timeout);
    this.connection = new WebSocket(this.url);

    this.connection.onopen = function () {
        this.attempts = 0;
        if (this.statusBubble !== null)
            this.statusBubble.style.backgroundColor = "green";
        TheFragebogen.logger.info("ScreenWaitWebsocket.connection.onopen()", "Connection opened.");
    }.bind(this);

    this.connection.onerror = function (error) {
        TheFragebogen.logger.error("ScreenWaitWebsocket.connection.onerror()", error);
    }.bind(this);

    this.connection.onmessage = function (e) {
        var msg = JSON.parse(e.data).data;
        TheFragebogen.logger.info("ScreenWaitWebsocket.connection.onmessage()", "Received: " + msg);
        if (this.msg === msg) {
            this.status = "Message received";
            document.dispatchEvent(new Event("nextScreen"));
        }
    }.bind(this);

    this.connection.onclose = function () {
        if (this.statusBubble != null)
            this.statusBubble.style.backgroundColor = "red";
        TheFragebogen.logger.info("ScreenWaitWebsocket.connection.onclose()", "Connection closed.");

        if (this.status != null) return;

        if (this.attempts == 10) {
            TheFragebogen.logger.error("ScreenWaitWebsocket.connection.onclose()", "Maximal number of attempts reached. ScreenWaitWebsocket will not try to reconnect again!");
            this._handleTimeout();
        }
        else {
            setTimeout(function () {
                this.attempts++;
                TheFragebogen.logger.info("ScreenWaitWebsocket.connection.onclose.setTimeout._anonymousFunction()", "Trying to reconnect...");
                this.start();
            }.bind(this), this.attempts * 500);
        }

        if (this.timeout != 0) {
            this.timeoutHandle = setTimeout((this._handleTimeout).bind(this), this.timeout);
        }
    }.bind(this);
    ScreenWaitWebsocket.prototype._handleTimeout = function () {
        TheFragebogen.logger.warn("ScreenWaitWebsocket._handleTimeout()", "Waiting got timeout after " + (!this.attempts ? (this.timeout + "ms.") : (this.attempts + " attempt(s).")));
        this.status = "Timeout reached";
        document.dispatchEvent(new Event("nextScreen"));
    };
    ScreenWaitWebsocket.prototype.createUI = function () {
        this.node = document.createElement("div");
        this.node.innerHTML = "Waiting server...<br>Status: ";

        this.node.className = this.className;

        this.statusBubble = document.createElement("div");
        this.statusBubble.setAttribute("style", "border-radius:50%;background-color:red;height:0.8em;width:0.8em;display:inline-flex");
        this.node.appendChild(this.statusBubble);

        return this.node;
    };
    ScreenWaitWebsocket.prototype.releaseUI = function () {
        this.node = null;
        this.statusBubble = null;

        clearTimeout(this.timeoutHandle);
        this.timeoutHandle = null;

        this.connection.onclose = function () {
            if (this.statusBubble != null)
                this.statusBubble.style.backgroundColor = "red";
            TheFragebogen.logger.info("ScreenWaitWebsocket.connection.onclose()", "Connection closed.");
        };

        this.connection.close();
        this.connection = null;
    };
    ScreenWaitWebsocket.prototype.getDataCSV = function () {
        return [["url", "status"], [this.url, this.status]];
    };
    ScreenWaitWebsocket.prototype.getData = function () {
        return [this.url, this.status];
    };
    ScreenWaitWebsocket.prototype._checkData = function (data) {
        if (typeof data[0] === "string" && typeof data[1] === "string")
            return true;
        else return false;
    };
    ScreenWaitWebsocket.prototype.setData = function (data) {
        if (!this._checkData(data)) return false;
        this.url = data[0];
        this.status = data[1];
    };
};
/**
 A screen that connects to a websocket-server and sends a message before continue.
 This screen sends a _nextScreen_-Event, when a signal via websocket is sent OR on timeout.

 @class ScreenWaitWebsocketSend
 @augments ScreenWait

 @param url The Websocket-URL to connect to.
 @param msg The message that should be received to dispatch event for next screen.
 @param timeout Automatically proceed, if the timeout is reached.
 */
function ScreenWaitWebsocketSend(className, url, msg, timeout) {
    ScreenWaitWebsocket.call(this, className, url, msg, timeout);
};

ScreenWaitWebsocketSend.prototype = Object.create(ScreenWaitWebsocket.prototype);
ScreenWaitWebsocketSend.prototype.start = function () {
    TheFragebogen.logger.warn("ScreenWaitWebsocketSend()", "Set: url as " + this.url + ", message as " + this.msg + " and timeout as " + this.timeout);
    this.connection = new WebSocket(this.url);

    this.connection.onopen = function () {
        this.attempts = 0;
        if (this.statusBubble !== null)
            this.statusBubble.style.backgroundColor = "green";
        TheFragebogen.logger.info("ScreenWaitWebsocketSend.connection.onopen()", "Sending: " + this.msg);
        this.connection.send(this.msg);
        this.status = "Message sent";
        document.dispatchEvent(new Event("nextScreen"));
    }.bind(this);

    this.connection.onmessage = function (e) {
        var msg = JSON.parse(e.data).data;
        TheFragebogen.logger.info("ScreenWaitWebsocketSend.connection.onmessage()", "Received: " + msg);
    }.bind(this);

    this.connection.onerror = function (error) {
        TheFragebogen.logger.error("ScreenWaitWebsocketSend.connection.onerror()", error);
    }.bind(this);

    this.connection.onclose = function () {
        if (this.statusBubble != null)
            this.statusBubble.style.backgroundColor = "red";
        TheFragebogen.logger.info("ScreenWaitWebsocketSend.connection.onclose()", "Connection closed.");

        if (this.status !== null) return;

        if (this.attempts > 10) {
            TheFragebogen.logger.error("ScreenWaitWebsocketSend.connection.onclose()", "Maximal number of attempts reached. ScreenWaitWebsocketSend will not try to reconnect again!");
            this._handleTimeout();
            return;
        }

        setTimeout(function () {
            this.attempts++;
            TheFragebogen.logger.info("ScreenWaitWebsocketSend.connection.onclose.setTimeout._anonymousFunction()", "Trying to reconnect...");
            this.start();
        }.bind(this), this.attempts * 500);
    }.bind(this);

    if (this.timeout !== 0) {
        this.timeoutHandle = setTimeout((this._handleTimeout).bind(this), this.timeout);
    }
};
/**
 Questionnaire item that waits until a defined weboscket message is received or timeout reached.
 No answer is provided or restored.
 It acts as a required item.

 @class QuestionnaireItemWaitWebsocket
 @augments QuestionnaireItem
 @param {string} [className]
 @param {string} url
 @param {string} [msg=""]
 @param {number} [timeout=0]
 TODO Actually this is not a questionnaireItem in sense of inheritance, but it gives an answer.
 */
function QuestionnaireItemWaitWebsocket(className, url, msg, timeout) {
    QuestionnaireItem.call(this, className, "", true);

    this.url = url;
    this.msg = typeof msg === "string" ? msg : "";
    this.timeout = !isNaN(timeout) ? Math.abs(timeout) * 1000 : 0;
    this.timeoutHandle;

    this.status = null;

    this.node = null;
    this.statusBubble = null;
    this.connection = null;
    this.attempts = 0;

    TheFragebogen.logger.warn("QuestionnaireItemWaitWebsocket():", "Set: url as " + this.url + ", message as" + this.msg + " and timeout as " + this.timeout);
}
QuestionnaireItemWaitWebsocket.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemWaitWebsocket.prototype.isReady = function () {
    return this.status != "waiting";
};
QuestionnaireItemWaitWebsocket.prototype._handleTimeout = function () {
    this._sendReadyCallback();
    TheFragebogen.logger.warn("QuestionnaireItemWaitWebsocket._handleTimeout()", "Waiting got timeout after " + (!this.attempts ? (this.timeout + "ms.") : (this.attempts + " attempt(s).")));
    this.status = "Timeout reached";
};
QuestionnaireItemWaitWebsocket.prototype.createUI = function () {
    this.connection = new WebSocket(this.url);

    this.connection.onopen = function () {
        this.attempts = 0;
        if (this.statusBubble != null)
            this.statusBubble.style.backgroundColor = "green";
        TheFragebogen.logger.info("QuestionnaireItemWaitWebsocket.connection.onopen()", "Connection opened.");
    }.bind(this);

    this.connection.onerror = function (error) {
        TheFragebogen.logger.error("QuestionnaireItemWaitWebsocket.connection.onerror()", error);
    }.bind(this);

    this.connection.onmessage = function (e) {
        var msg = JSON.parse(e.data).data;
        TheFragebogen.logger.info("QuestionnaireItemWaitWebsocket.connection.onmessage()", "Received: " + msg);
        if (this.msg === msg) {
            this.status = "Message received";
            this._sendReadyCallback();
        }
    }.bind(this);

    this.connection.onclose = function () {
        if (this.statusBubble != null)
            this.statusBubble.style.backgroundColor = "red";
        TheFragebogen.logger.info("QuestionnaireItemWaitWebsocket.connection.onclose()", "Connection closed.");

        if (this.status != null) return;

        if (this.attempts > 10) {
            TheFragebogen.logger.error("QuestionnaireItemWaitWebsocket.connection.onclose()", "Maximal number of attempts reached. QuestionnaireItemWaitWebsocket will not try to reconnect again!");
            this._handleTimeout();
            return;
        }
        setTimeout(function () {
            this.attempts++;
            TheFragebogen.logger.info("QuestionnaireItemWaitWebsocket.connection.onclose.setTimeout._anonymousFunction()", "Trying to reconnect...");
            this.start();
        }.bind(this), this.attempts * 500);
    }.bind(this);

    if (this.timeout != 0) {
        this.timeoutHandle = setTimeout((this._handleTimeout).bind(this), this.timeout);
    }

    this.node = document.createElement("div");

    this.statusBubble = document.createElement("div");
    this.statusBubble.setAttribute("style", "border-radius:50%;background-color:red;height:0.8em;width:0.8em;display:inline-flex");
    this.node.appendChild(this.statusBubble);
    this.node.className = this.className;

    this.status = "waiting";

    return this.node;
};
QuestionnaireItemWaitWebsocket.prototype.releaseUI = function () {
    this.node = null;
    this.statusBubble = null;

    clearTimeout(this.timeoutHandle);
    this.timeoutHandle = null;

    this.connection.onclose = function () {
        if (this.statusBubble != null)
            this.statusBubble.style.backgroundColor = "red";
        TheFragebogen.logger.info("QuestionnaireItemWaitWebsocket.connection.onclose()", "Connection closed.");
    };

    this.connection.close();
    this.connection = null;
};
/**
 7pt Quality scale as defined in ITU-T P.851 p. 19 using a SVG.<br>
 Valid answer range is 10 to 70 with step value 1.<br>
 <br>
 It is implementated using two SVGs (one outer for the size and the actual content inline).<br>
 The inline SVG contains clickable areas and the "id" contains the value for the answer.<br>
 Moreover the labels are named "labelXX" where XX denotes the value and can be changed using Javascript.<br>
 See example before inserting any custom CSS code.<br>
 <br>
 PS: It should be inherited by the QuestionnaireItemDefined and its labels changed according to an optionList of 7 strings in the future.
 @example
 Basic HTML Output:
 <div>
 <div>Question node</div>
 <div class="QuestionnaireItemQuality7pt">
 <svg></svg>
 </div>
 </div>
 @class QuestionnaireItemQuality7pt
 @augments UIElement
 @augments UIElementInteractive
 @augments QuestionnaireItem
 @param {string} [className] CSS-style
 @param {string} question question
 @param {boolean} [required=false] if the item must be answered
 */
function QuestionnaireItemQuality7pt(className, question, required) {
    QuestionnaireItem.call(this, className, question, required);

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
}
QuestionnaireItemQuality7pt.prototype = Object.create(QuestionnaireItem.prototype); //TODO Move to QuestionnaireItemDefinedOne
QuestionnaireItemQuality7pt.prototype.constructor = QuestionnaireItemQuality7pt;
/**
 Creates the node of the answer
 @returns {object} node of the answer
 */
QuestionnaireItemQuality7pt.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    node.className = "QuestionnaireItemQuality7pt"; //TODO Why?

    this.scaleImage = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    this.scaleImage.setAttribute("viewBox", "0 2 136.76 21.39");
    this.scaleImage.setAttribute("height", "150px");
    svg = '<?xml version="1.0" encoding="UTF-8" standalone="no"?><!-- Created with Inkscape (http://www.inkscape.org/) --><svg   xmlns:dc="http://purl.org/dc/elements/1.1/"   xmlns:cc="http://creativecommons.org/ns#"   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"   xmlns:svg="http://www.w3.org/2000/svg"   xmlns="http://www.w3.org/2000/svg"   version="1.1"   id="svg5198">  <defs     id="defs5200" />  <metadata     id="metadata5203">    <rdf:RDF>      <cc:Work         rdf:about="">        <dc:format>image/svg+xml</dc:format>        <dc:type           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />        <dc:title></dc:title>      </cc:Work>    </rdf:RDF>  </metadata>  <g     transform="translate(-12.104855,-1030.0402)"     id="layer1"     style="display:inline">    <rect       width="103"       height="1"       x="30"       y="1036.3621"       id="rect5206"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="25"       y="1037.2622"       id="rect5763"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="29.000103"       y="1037.2622"       id="rect5765"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="33.000206"       y="1037.2622"       id="rect5765-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="36.999847"       y="1037.2622"       id="rect5765-9-4"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.2"       height="5"       x="40.799999"       y="1037.3622"       id="rect5822"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="45"       y="1037.2622"       id="rect5763-5"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="49.000103"       y="1037.2622"       id="rect5765-7"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="53.000206"       y="1037.2622"       id="rect5765-9-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="56.999847"       y="1037.2622"       id="rect5765-9-4-2"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.2"       height="5"       x="60.799999"       y="1037.3622"       id="rect5822-6"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="65"       y="1037.2622"       id="rect5763-5-2"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="69.000107"       y="1037.2622"       id="rect5765-7-5"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="73.000206"       y="1037.2622"       id="rect5765-9-9-0"       style="fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="76.999847"       y="1037.2622"       id="rect5765-9-4-2-3"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.2"       height="5"       x="80.800003"       y="1037.3622"       id="rect5822-6-5"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="85.000008"       y="1037.2622"       id="rect5763-5-2-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="89.000114"       y="1037.2622"       id="rect5765-7-5-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="93.000214"       y="1037.2622"       id="rect5765-9-9-0-0"       style="fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="96.999855"       y="1037.2622"       id="rect5765-9-4-2-3-5"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.2"       height="5"       x="100.8"       y="1037.3622"       id="rect5822-6-5-4"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="105.00002"       y="1037.2622"       id="rect5763-5-2-9-6"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="109.00013"       y="1037.2622"       id="rect5765-7-5-9-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="113.00023"       y="1037.2622"       id="rect5765-9-9-0-0-8"       style="fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="116.99986"       y="1037.2622"       id="rect5765-9-4-2-3-5-0"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.2"       height="5"       x="120.8"       y="1037.3622"       id="rect5822-6-5-4-7"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="125.00002"       y="1037.2622"       id="rect5763-5-2-9-6-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="129.00012"       y="1037.2622"       id="rect5765-7-5-9-9-9"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="133.00023"       y="1037.2622"       id="rect5765-9-9-0-0-8-0"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="136.99986"       y="1037.2622"       id="rect5765-9-4-2-3-5-0-5"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="8.8000002"       height="0.40000001"       x="21.204855"       y="1036.6622"       id="rect6036"       style="fill:#000000;fill-opacity:1" />    <rect       width="0.3491767"       height="5.4000001"       x="21.206226"       y="1036.9623"       id="rect6036-5"       style="fill:#000000;fill-opacity:1" />    <rect       width="8.8000002"       height="0.40000001"       x="-141.80486"       y="1036.6621"       transform="scale(-1,1)"       id="rect6036-2"       style="fill:#000000;fill-opacity:1" />    <rect       width="0.40000001"       height="5.4000001"       x="-141.80486"       y="1036.9622"       transform="scale(-1,1)"       id="rect6036-5-2"       style="fill:#000000;fill-opacity:1" />    <text       x="21.174191"       y="1044.4059"       id="label10"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="21.174191"         y="1044.4059"         id="tspan3851">extrem</tspan><tspan         x="21.174191"         y="1046.4059"         id="tspan3853">schlecht</tspan></text>    <text       x="41.174191"       y="1042.5059"       id="label20"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="41.174191"         y="1042.5059"         id="tspan3851-5" /><tspan         x="41.174191"         y="1044.5059"         id="tspan3853-8">schlecht</tspan></text>    <text       x="61.267941"       y="1042.6182"       id="label30"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="61.267941"         y="1042.6182"         id="tspan3851-5-5" /><tspan         x="61.267941"         y="1044.6182"         id="tspan3853-8-1">dürftig</tspan></text>    <text       x="81.267944"       y="1042.6058"       id="label30-6"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="81.267944"         y="1042.6058"         id="tspan3851-5-5-2" /><tspan         x="81.267944"         y="1044.6058"         id="tspan3853-8-1-6">ordentlich</tspan></text>    <text       x="101.4683"       y="1042.4182"       id="label30-6-5"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="101.4683"         y="1042.4182"         id="tspan3851-5-5-2-6" /><tspan         x="101.4683"         y="1044.4182"         id="tspan3853-8-1-6-0">gut</tspan></text>    <text       x="121.25037"       y="1042.5182"       id="label30-6-0"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="121.25037"         y="1042.5182"         id="tspan3851-5-5-2-1" /><tspan         x="121.25037"         y="1044.5182"         id="tspan3853-8-1-6-04">ausgezeichnet</tspan></text>    <text       x="141.63435"       y="1042.5059"       id="label30-6-0-5"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="141.63435"         y="1042.5059"         id="tspan3851-5-5-2-1-6" /><tspan         x="141.63435"         y="1044.5059"         id="tspan3853-8-1-6-04-3">ideal</tspan></text>  </g>  <g     transform="translate(7.8951471,6.3219508)"     id="layer3"     style="display:inline">    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,5.3372074,-0.46569083)"       id="12"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,7.3930898,-0.46569085)"       id="13"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,9.3930898,-0.46569085)"       id="14"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,11.39309,-0.46569083)"       id="15"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,13.39309,-0.46569083)"       id="16"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,15.39309,-0.46569083)"       id="17"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,17.39309,-0.46569083)"       id="18"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,19.39309,-0.46569083)"       id="19"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,21.29309,-0.46569083)"       id="20"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,25.337207,-0.46569083)"       id="22"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,27.393089,-0.46569083)"       id="23"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,29.393089,-0.46569083)"       id="24"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,31.393089,-0.46569083)"       id="25"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,33.393089,-0.46569083)"       id="26"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,35.39309,-0.46569083)"       id="27"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,37.39309,-0.46569083)"       id="28"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,39.39309,-0.46569083)"       id="29"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,41.29309,-0.46569083)"       id="30"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,23.24309,-0.46569083)"       id="21"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,45.337207,-0.46569083)"       id="32"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,47.393089,-0.46569083)"       id="33"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,49.393089,-0.46569083)"       id="34"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,51.393089,-0.46569083)"       id="35"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,53.393089,-0.46569083)"       id="36"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,55.39309,-0.46569083)"       id="37"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,57.39309,-0.46569083)"       id="38"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,59.39309,-0.46569083)"       id="39"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,61.29309,-0.46569083)"       id="40"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,43.24309,-0.46569083)"       id="31"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,65.337207,-0.46569083)"       id="42"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,67.393089,-0.46569083)"       id="43"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,69.393089,-0.46569083)"       id="44"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,71.393089,-0.46569083)"       id="45"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,73.393089,-0.46569083)"       id="46"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,75.39309,-0.46569083)"       id="47"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,77.39309,-0.46569083)"       id="48"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,79.39309,-0.46569083)"       id="49"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,81.29309,-0.46569083)"       id="50"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,63.24309,-0.46569083)"       id="41"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,85.337207,-0.46569083)"       id="52"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,87.393089,-0.46569083)"       id="53"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,89.393089,-0.46569083)"       id="54"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,91.393089,-0.46569083)"       id="55"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,93.393089,-0.46569083)"       id="56"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,95.39309,-0.46569083)"       id="57"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,97.39309,-0.46569083)"       id="58"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,99.39309,-0.46569083)"       id="59"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,101.29309,-0.46569083)"       id="60"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,83.24309,-0.46569083)"       id="51"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.62745098,0,0,0.68817204,105.38721,-0.46569083)"       id="62"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,107.44309,-0.46569083)"       id="63"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,109.44309,-0.46569083)"       id="64"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,111.44309,-0.46569083)"       id="65"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,113.44309,-0.46569083)"       id="66"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,115.44309,-0.46569083)"       id="67"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,117.44309,-0.46569083)"       id="68"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,119.44309,-0.46569083)"       id="69"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,121.44309,-0.46569083)"       id="70"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,103.29309,-0.46569083)"       id="61"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,3.2930898,-0.46569083)"       id="11"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.59607843,0,0,0.68817204,1.2930898,-0.46569083)"       id="10"       style="opacity:0;fill:#000000;fill-opacity:0.45871558" />  </g>  <g     transform="translate(7.0000016,5.4565456)"     id="layer4"     style="display:inline">    <path       d="M 3.666497,-0.09404561 C 0.69774682,2.8434544 0.69774682,2.8434544 0.69774682,2.8434544 L 2.2289971,1.3747044 0.72899682,-0.15654561 3.697747,2.8434544"       id="cross"       style="fill:none;stroke:#000000;stroke-width:0.60000002;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none" />  </g></svg>';
    var svgDoc = new DOMParser().parseFromString(svg, "text/xml");
    this.scaleImage.appendChild(svgDoc.documentElement);

    this.answerMap = {};
    var answerElements = this.scaleImage.getElementsByTagName("path"); //Attach event listener to clickable areas.
    for (var i = 0; i < answerElements.length; i++) {
        if (answerElements[i].id < 10 || answerElements[i].id > 70) {
            TheFragebogen.logger.warn("QuestionnaireItemQuality7pt._createAnswerNode()", "Invalid id (10 <= id <= 70): " + answerElements[i].id);
            continue;
        }
        this.answerMap[answerElements[i].id] = answerElements[i];
        answerElements[i].onclick = (this._handleChange).bind(this);
    }

    this.crossImage = this.scaleImage.getElementById("cross");
    //Problem identified here by the tests while using Safari 7.0.6 --- this.crossImage === null
    this.crossImage.setAttributeNS(null, "opacity", 0);

    if (this.answer != null) this._updateUI();

    node.appendChild(this.scaleImage);
    return node;
};
//TODO document it after documenting the method in the questionnaire_item_basic.js file
QuestionnaireItemQuality7pt.prototype._handleChange = function (event) {
    if (!this.isEnabled()) return;

    this.setAnswer(event.target.id);

    TheFragebogen.logger.info("QuestionnaireItemQuality7pt._handleChange()", this.getAnswer());

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
};
/**
 Called when the 7pt. Quality Scale UI should be updated.
 Updates the UI.
 */
//TODO What does it return, nothing or a boolean?
QuestionnaireItemQuality7pt.prototype._updateUI = function () {
    if (!this.isUIcreated()) return;

    if (this.answer == null) {
        this.crossImage.setAttributeNS(null, "opacity", 0);
        return;
    }
    if (this.answerMap[this.getAnswer()] == undefined) {
        TheFragebogen.logger.error("QuestionnaireItemQuality7pt._updateUI()", "Invalid answer provided: " + this.getAnswer());
        return false;
    }

    //Displays cross
    this.crossImage.setAttributeNS(null, "opacity", 1);

    var transform = this.answerMap[this.answer].getTransformToElement(this.crossImage.parentElement)
    TheFragebogen.logger.info("QuestionnaireItemQuality7pt._updateUI()", transform.e + " " + transform.f);
    this.crossImage.setAttributeNS(null, "transform", "translate(" + (transform.e - this.crossImage.getBBox().width / 2 - 0.6) + "," + (transform.f - 0.4) + ")"); //Remark: 0.6 and 0.4 are magic.
};
/**
 Sets the answer.
 @param {string} answer answer, must be an integer value between 10 and 70
 @returns {boolean} true if the answer was set
 */
QuestionnaireItemQuality7pt.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._updateUI();
        return true;
    }
    if (answer < 10 || answer > 70) {
        TheFragebogen.logger.error("QuestionnaireItemQuality7pt.setAnswer()", "Invalid answer provided: " + answer);
        return false;
    }

    TheFragebogen.logger.info("QuestionnaireItemQuality7pt.setAnswer()", answer);
    this.answer = answer;

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
    return true;
};
/**
 Removes all references from and to elements created by createUI().
 */
QuestionnaireItemQuality7pt.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;
    this.enabled = false;

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
};
/**
 Returns the question and answer of the element.
 @returns {array} an array containing the question in the index 0 and the answer in the index 1
 */
QuestionnaireItemQuality7pt.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
/**
 Compares the question of the provided input data array and the question stored
 in the element.
 @param {array} data question stored in the index 0 of the array
 @returns {boolean} true if it is the same question
 */
QuestionnaireItemQuality7pt.prototype._checkData = function (data) {
    return data[0] == this.question;
};
/**
 Sets the answer of the provided input data as the answer of the element.<br>
 The question of the provided input data must be the same as the question stored
 in the element.
 @param {array} data question and answer stored in the indexes 0 and 1 of the array
 @returns {boolean} true if the input data answer was set
 */
QuestionnaireItemQuality7pt.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};
/**
 Returns the range of values that the item can assume.
 @returns {string} "10-70", possible values assumed by the item
 */
QuestionnaireItemQuality7pt.prototype.getOptionList = function () {
    return "10-70";
};
/**
 100pt visual analogue scale with two labels.
 Valid answer range is 10 to 109 in 1 step.

 It is implementated using two SVGs (one outer for the size and the actual content inline).
 The inline SVG contains clickable areas and the "id" contains the value for the answer.
 Moreover the labels are named "labelLeft" and "labelRight".

 @class QuestionnaireItemVisualAnalogueScale
 @augments QuestionnaireItem
 */
function QuestionnaireItemVisualAnalogueScale(className, question, required, captionLeft, captionRight) {
    QuestionnaireItem.call(this, className, question, required);

    this.captionLeft = captionLeft;
    this.captionRight = captionRight;

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
}
QuestionnaireItemVisualAnalogueScale.prototype = Object.create(QuestionnaireItem.prototype); //TODO Move to QuestionnaireItemDefinedOne
QuestionnaireItemVisualAnalogueScale.prototype.constructor = QuestionnaireItemVisualAnalogueScale;
QuestionnaireItemVisualAnalogueScale.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    this.scaleImage = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    this.scaleImage.setAttribute("viewBox", "0 2 136.76 19.39");
    this.scaleImage.innerHTML = '<?xml version="1.0" encoding="UTF-8" standalone="no"?><!-- Created with Inkscape (http://www.inkscape.org/) --><svg   xmlns:dc="http://purl.org/dc/elements/1.1/"   xmlns:cc="http://creativecommons.org/ns#"   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"   xmlns:svg="http://www.w3.org/2000/svg"   xmlns="http://www.w3.org/2000/svg"   xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"   version="1.1"   width="136.76094"   height="19.389999"   id="svg5198">  <defs     id="defs5200" />  <metadata     id="metadata5203">    <rdf:RDF>      <cc:Work         rdf:about="">        <dc:format>image/svg+xml</dc:format>        <dc:type           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />        <dc:title />      </cc:Work>    </rdf:RDF>  </metadata>  <g     transform="translate(-12.104855,-1032.0442)"     id="layer1"     style="display:inline">    <rect       width="120.59571"       height="0.84977901"       x="21.227472"       y="1040.2581"       transform="matrix(0.99999922,0.00124566,0,1,0,0)"       id="rect5206"       style="fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="25"       y="1041.2622"       id="rect5763"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="29.000103"       y="1041.2622"       id="rect5765"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1.000026"       height="2.0999999"       x="133.00023"       y="1041.2622"       id="rect5765-9-9-0-0-8-0"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       width="1"       height="2.0999999"       x="136.99986"       y="1041.2622"       id="rect5765-9-4-2-3-5-0-5"       style="opacity:0;fill:#000000;fill-opacity:1" />    <text       x="6.717514"       y="18.240952"       transform="translate(12.104855,1032.0442)"       id="text4739"       xml:space="preserve"       style="font-size:40px;font-style:normal;font-weight:normal;line-height:125%;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans"><tspan         x="6.717514"         y="18.240952"         id="tspan4741" /></text>    <text       x="18.967247"       y="1041.3994"       id="labelLeft"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="18.967247"         y="1041.3994"         id="tspan3853"         style="text-align:end;text-anchor:end">left</tspan></text>    <text       x="142.89552"       y="1041.4407"       id="labelRight"       xml:space="preserve"       style="font-size:2px;font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;text-align:start;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:start;fill:#000000;fill-opacity:1;stroke:none;display:inline;font-family:Sans;-inkscape-font-specification:Sans"><tspan         x="142.89552"         y="1041.4407"         id="tspan3853-3"         style="text-align:start;text-anchor:start">right</tspan></text>  </g>  <g     transform="translate(7.8951471,4.3179676)"     id="g3179"     style="display:inline">    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,52.481031,1.490115)"       id="52"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,53.730062,1.490115)"       id="53"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,54.945144,1.490115)"       id="54"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,56.160227,1.490115)"       id="55"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,57.375308,1.490115)"       id="56"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,58.59039,1.490115)"       id="57"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,59.805471,1.490115)"       id="58"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,61.020552,1.490115)"       id="59"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,62.174882,1.490115)"       id="60"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,64.631848,1.490115)"       id="62"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,65.880879,1.490115)"       id="63"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,67.095961,1.490115)"       id="64"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,68.311043,1.490115)"       id="65"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,69.526124,1.490115)"       id="66"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,70.741207,1.490115)"       id="67"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,71.956289,1.490115)"       id="68"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,73.17137,1.490115)"       id="69"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,74.325698,1.490115)"       id="70"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,63.359585,1.490115)"       id="61"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,76.782664,1.490115)"       id="72"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,78.031697,1.490115)"       id="73"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,79.246779,1.490115)"       id="74"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,80.461861,1.490115)"       id="75"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,81.676942,1.490115)"       id="76"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,82.892024,1.490115)"       id="77"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,84.107105,1.490115)"       id="78"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,85.322188,1.490115)"       id="79"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,86.476516,1.490115)"       id="80"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,75.510403,1.490115)"       id="71"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,88.933481,1.490115)"       id="82"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,90.182513,1.490115)"       id="83"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,91.397596,1.490115)"       id="84"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,92.612677,1.490115)"       id="85"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,93.827759,1.490115)"       id="86"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,95.042841,1.490115)"       id="87"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,96.257922,1.490115)"       id="88"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,97.473006,1.490115)"       id="89"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,98.627332,1.490115)"       id="90"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,87.66122,1.490115)"       id="81"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,101.0843,1.490115)"       id="92"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,102.33333,1.490115)"       id="93"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,103.54842,1.490115)"       id="94"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,104.7635,1.490115)"       id="95"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,105.97859,1.490115)"       id="96"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,107.19366,1.490115)"       id="97"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,108.40875,1.490115)"       id="98"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,109.62383,1.490115)"       id="99"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,110.77817,1.490115)"       id="100"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,99.812037,1.490115)"       id="91"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,113.26549,1.490115)"       id="102"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,114.51452,1.490115)"       id="103"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,115.72962,1.490115)"       id="104"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,116.94469,1.490115)"       id="105"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,118.15979,1.490115)"       id="106"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,119.37485,1.490115)"       id="107"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,120.58993,1.490115)"       id="108"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,121.80502,1.490115)"       id="109"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,111.99323,1.490115)"       id="101"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,51.239145,1.490115)"       id="51"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,50.024063,1.490115)"       id="50"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,3.6950017,1.5342684)"       id="12"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,4.9440331,1.5342683)"       id="13"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,6.1591143,1.5342683)"       id="14"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,7.3741961,1.5342684)"       id="15"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,8.5892781,1.5342684)"       id="16"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,9.8043599,1.5342684)"       id="17"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,11.019442,1.5342684)"       id="18"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,12.234523,1.5342684)"       id="19"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,13.388851,1.5342684)"       id="20"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,15.845817,1.5342684)"       id="22"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,17.09485,1.5342684)"       id="23"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,18.309933,1.5342684)"       id="24"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,19.525013,1.5342684)"       id="25"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,20.740096,1.5342684)"       id="26"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,21.955178,1.5342684)"       id="27"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,23.170259,1.5342684)"       id="28"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,24.385342,1.5342684)"       id="29"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,25.539668,1.5342684)"       id="30"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,14.573557,1.5342684)"       id="21"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,29.333721,1.4900742)"       id="33"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,30.582754,1.4900742)"       id="34"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,31.797837,1.4900742)"       id="35"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,33.012917,1.4900742)"       id="36"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,34.228,1.4900742)"       id="37"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,35.443082,1.4900742)"       id="38"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,36.658163,1.4900742)"       id="39"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,37.873246,1.4900742)"       id="40"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,39.027572,1.4900742)"       id="41"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,28.061461,1.4900742)"       id="32"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.38120211,0,0,0.68817204,41.484539,1.4900742)"       id="43"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.1714536;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,42.733571,1.4900742)"       id="44"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,43.948656,1.4900742)"       id="45"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,45.163736,1.4900742)"       id="46"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,46.378819,1.4900742)"       id="47"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,47.593902,1.4900742)"       id="48"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,48.808984,1.4900742)"       id="49"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,40.212277,1.4900742)"       id="42"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,2.4531152,1.5342684)"       id="11"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,1.2380334,1.5342684)"       id="10"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />    <path       d="m 1.78125,2.9611573 a 1.59375,3.6328125 0 1 1 -3.1875,0 1.59375,3.6328125 0 1 1 3.1875,0 z"       transform="matrix(0.36214201,0,0,0.68817204,26.751473,1.4995672)"       id="31"       style="opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none;display:inline" />  </g>  <g     transform="translate(7.0000016,3.4525612)"     id="layer4"     style="display:inline">    <path       d="m 3.9843731,3.6481463 c -2.9687502,2.9375 -2.9687502,2.9375 -2.9687502,2.9375 l 1.5312503,-1.46875 -1.5000003,-1.53125 2.9687502,3"       inkscape:connector-curvature="0"       id="cross"       style="fill:none;stroke:#000000;stroke-width:0.60000002;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none;display:inline" />  </g></svg>';

    this.answerMap = {};
    var answerElements = this.scaleImage.getElementsByTagName("path"); //Attach event listener to clickable areas.
    for (var i = 0; i < answerElements.length; i++) {
        if (answerElements[i].id < 10 || answerElements[i].id > 109) {
            TheFragebogen.logger.warn("QuestionnaireItemVisualAnalogueScale._createAnswerNode()", "Invalid id (10 <= id <= 109): " + answerElements[i].id);
            continue;
        }
        this.answerMap[answerElements[i].id] = answerElements[i];
        answerElements[i].onclick = (this._handleChange).bind(this);
    }

    this.scaleImage.getElementById("labelLeft").textContent = this.captionLeft;
    this.scaleImage.getElementById("labelRight").textContent = this.captionRight;

    this.crossImage = this.scaleImage.getElementById("cross");
    //Problem identified here by the tests while using Safari 7.0.6 --- this.crossImage === null
    this.crossImage.setAttributeNS(null, "opacity", 0);

    if (this.answer != null) this._updateUI();

    node.appendChild(this.scaleImage);
    return node;
};
QuestionnaireItemVisualAnalogueScale.prototype._handleChange = function (event) {
    if (!this.isEnabled()) return;

    this.setAnswer(event.target.id);

    TheFragebogen.logger.info("QuestionnaireItemVisualAnalogueScale._handleChange()", this.getAnswer());

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
};
/**
 Called when the 7pt. Quality Scale UI should be updated.<br>
 Update the UI.
 */
QuestionnaireItemVisualAnalogueScale.prototype._updateUI = function () {
    if (!this.isUIcreated()) return;

    if (this.answer == null) {
        this.crossImage.setAttributeNS(null, "opacity", 0);
        return;
    }
    if (this.answerMap[this.getAnswer()] == undefined) {
        TheFragebogen.logger.error("QuestionnaireItemVisualAnalogueScale._updateUI()", "Invalid answer provided: " + this.getAnswer());
        return false;
    }

    //Displays cross
    this.crossImage.setAttributeNS(null, "opacity", 1);

    var transform = this.answerMap[this.answer].getTransformToElement(this.crossImage.parentElement)
    TheFragebogen.logger.info("QuestionnaireItemVisualAnalogueScale._updateUI()", transform.e + " " + transform.f);
    this.crossImage.setAttributeNS(null, "transform", "translate(" + (transform.e - this.crossImage.getBBox().width / 2 - 0.6) + "," + (transform.f - 2.0) + ")"); //Remark: 0.6 and 0.4 are magic.
};
QuestionnaireItemVisualAnalogueScale.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._updateUI();
        return true;
    }
    if (answer < 10 || answer > 109) {
        TheFragebogen.logger.error("QuestionnaireItemVisualAnalogueScale.setAnswer()", "Invalid answer provided: " + answer);
        return false;
    }

    TheFragebogen.logger.info("QuestionnaireItemVisualAnalogueScale.setAnswer()", answer);
    this.answer = answer;

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
    return true;
};
QuestionnaireItemVisualAnalogueScale.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
};
QuestionnaireItemVisualAnalogueScale.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
QuestionnaireItemVisualAnalogueScale.prototype._checkData = function (data) {
    return data[0] == this.question;
};
QuestionnaireItemVisualAnalogueScale.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};
QuestionnaireItemVisualAnalogueScale.prototype.getOptionList = function (data) {
    return "10-109";
};
/**
 A screen playing a video in full-screen.<br>
 Controls are not provided.<br>
 See example before inserting any custom CSS code.
 @example
 Basic HTML Output:
 <video></video>
 @class ScreenVideoPlayerWebsocket
 @augments Screen
 @param url The URL of the video.
 @param [interval] The interval to send the progress.
 @param [hashmap] The hashmap containing the the each "time-reached" and message to be sent on progress.
 */
 
 //TODO Refactor to inherit ScreenMediaVideo
function ScreenVideoPlayerWebsocket(url, interval, hashmap, websocketURL) {
    Screen.call(this);

    this.url = url;
    this.interval = interval;
    this.intervalCount = 0;
    this.hashmap = hashmap instanceof Object ? hashmap : {};
    this.hashmapTimes = Object.keys(this.hashmap);
    this.websocketURL = websocketURL;

    if (this.interval != undefined || this.hashmap != undefined)
        this.ws = new ReconnectingWebSocket(this.websocketURL);

    this.played = false;

    this.video = document.createElement('video');
    this.video.src = this.url;
    this.video.addEventListener("canplaythrough", function () {
        this.isMediaLoaded = function () {
            return true;
        }
    }.bind(this), true);
}
ScreenVideoPlayerWebsocket.prototype = Object.create(Screen.prototype);
ScreenVideoPlayerWebsocket.prototype.constructor = ScreenVideoPlayerWebsocket;
ScreenVideoPlayerWebsocket.prototype.createUI = function () {
    this.video.style.position = "absolute";
    this.video.style.left = "0px";
    this.video.style.top = "0px";
    this.video.style.height = "100%";
    this.video.style.width = "100%";
    this.video.style.backgroundColor = "black";

    this.video.onended = (this._ended).bind(this);
    this.video.ontimeupdate = (this._timeUpdate).bind(this);

    return this.video;
};
ScreenVideoPlayerWebsocket.prototype.start = function () {
    this.video.play();
};
ScreenVideoPlayerWebsocket.prototype._timeUpdate = function () {
    var screen = this;
    var currentTime = this.video.currentTime;

    if (this.interval > 0 && Math.abs(currentTime % this.interval) <= 0.3)
        if (this.ws.readyState == 1)
            this.ws.send("goto " + (this.intervalCount * this.interval)); //TODO send what?
        else
            TheFragebogen.logger.error("ScreenVideoPlayer._timeupdate", "Could not send message because the connection was not established.");

    var time = this.hashmapTimes.filter(function (element) {
        var relativeTime = currentTime - element;
        return Math.abs(relativeTime) <= 0.15;
    });

    if (time.length == 1 && this.ws.readyState == 1) {
        for (var i in time)
            this.ws.send("goto " + screen.hashmap[i]);
    } else {
        TheFragebogen.logger.error("ScreenVideoPlayer._timeupdate", "Could not send message because the connection was not established.");
    }
};
ScreenVideoPlayerWebsocket.prototype._ended = function () {
    this.played = true;
    document.dispatchEvent(new Event("nextScreen"));
};
ScreenVideoPlayerWebsocket.prototype.isReady = function () {
    return this.played;
};
ScreenVideoPlayerWebsocket.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;

    if (this.ws != undefined) this.ws.close();
    this.played = false;
    if (this.video != null) this.video.pause();
    this.video = null;
};

ScreenVideoPlayerWebsocket.prototype.getData = function () {
    return [this.url];
};
ScreenVideoPlayerWebsocket.prototype._checkData = function (data) {
    return (data[0] == this.url);
};
ScreenVideoPlayerWebsocket.prototype.setData = function (data) {
    return this._checkData(data);
};
ScreenVideoPlayerWebsocket.prototype.isMediaLoaded = function () {
    return false;
};
/**

NASA Task Load Index

from http://humansystems.arc.nasa.gov/groups/tlx/downloads/TLXScale.pdf
(see also the manual at http://humansystems.arc.nasa.gov/groups/tlx/downloads/TLX_pappen_manual.pdf )

Hart and Staveland's NASA Task Load Index (TLX) method asses
work load on five 7-point scales. Increments of high, medium
and low estimates for each point result in 21 gradations on
the scales.

This Questionnaire Item should be created five times with the
appropriate dimensions.

 @author Maxim Spur
 @class QuestionnaireItemNASATLXScale
 @augments QuestionnaireItem
 */
function QuestionnaireItemNASATLXScale(className, question, required, captionLeft, captionRight) {
    QuestionnaireItem.call(this, className, question, required);

    this.captionLeft = captionLeft;
    this.captionRight = captionRight;

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
}
QuestionnaireItemNASATLXScale.prototype = Object.create(QuestionnaireItem.prototype);
QuestionnaireItemNASATLXScale.prototype.constructor = QuestionnaireItemNASATLXScale;

QuestionnaireItemNASATLXScale.prototype._createAnswerNode = function () {
    var node = document.createElement("div");

    node.className = "QuestionnaireItemNASATLXScale";

    this.scaleImage = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    this.scaleImage.setAttribute("viewBox", "0 5 115 20");
    this.scaleImage.innerHTML = '<?xml version="1.0" encoding="UTF-8" standalone="no"?><svg   xmlns:dc="http://purl.org/dc/elements/1.1/"   xmlns:cc="http://creativecommons.org/ns#"   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"   xmlns:svg="http://www.w3.org/2000/svg"   xmlns="http://www.w3.org/2000/svg"   xmlns:xlink="http://www.w3.org/1999/xlink"   width="115"   height="22.750004"   id="svg5198"   version="1.1">  <defs     id="defs5200" />  <metadata     id="metadata5203">    <rdf:RDF>      <cc:Work         rdf:about="">        <dc:format>image/svg+xml</dc:format>        <dc:type           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />        <dc:title></dc:title>      </cc:Work>    </rdf:RDF>  </metadata>  <g     style="display:inline"     transform="translate(-14.754855,-1027.4342)"     id="layer1">    <rect       y="1041.2622"       x="25"       height="2.0999999"       width="1.000026"       id="rect5763"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       y="1041.2622"       x="29.000103"       height="2.0999999"       width="1.000026"       id="rect5765"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       y="1041.2622"       x="133.00023"       height="2.0999999"       width="1.000026"       id="rect5765-9-9-0-0-8-0"       style="opacity:0;fill:#000000;fill-opacity:1" />    <rect       y="1041.2622"       x="136.99986"       height="2.0999999"       width="1"       id="rect5765-9-4-2-3-5-0-5"       style="opacity:0;fill:#000000;fill-opacity:1" />    <text       transform="translate(12.104855,1032.0442)"       id="text4739"       y="18.240952"       x="6.717514"       style="font-style:normal;font-weight:normal;font-size:40px;line-height:125%;font-family:Sans;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none"       xml:space="preserve"><tspan         y="18.240952"         x="6.717514"         id="tspan4741" /></text>    <text       id="labelLeft"       y="1045.1559"       x="30"       style="font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:3px;line-height:100%;font-family:Sans;-inkscape-font-specification:Sans;text-align:center;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;display:inline;fill:#000000;fill-opacity:1;stroke:none"       xml:space="preserve"><tspan         style="text-align:start;text-anchor:start"         id="tspan3853"         y="1045.1559"         x="30">left</tspan></text>    <text       id="labelRight"       y="1044.7682"       x="105"       style="font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:3px;line-height:100%;font-family:Sans;-inkscape-font-specification:Sans;text-align:start;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:start;display:inline;fill:#000000;fill-opacity:1;stroke:none"       xml:space="preserve"><tspan         style="text-align:end;text-anchor:end"         id="tspan3853-3"         y="1044.7682"         x="115">right</tspan></text>    <path       id="path4250"       d="m 22.104855,1041.2842 99.999995,0"       style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0.30000001;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1" />    <path       id="path4252"       d="m 22.204855,1041.4342 0,-5"       style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0.23783921;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1" />    <use       height="100%"       width="100%"       id="use5759"       transform="translate(5,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5761"       transform="translate(10,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5763"       transform="translate(15,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5765"       transform="translate(20,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5767"       transform="translate(25,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5769"       transform="translate(30,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5771"       transform="translate(35,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5773"       transform="translate(40,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5775"       transform="translate(45,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5777"       transform="matrix(1,0,0,1.6,50,-624.86052)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5779"       transform="translate(55,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5781"       transform="translate(60,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5783"       transform="translate(65,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5785"       transform="translate(70,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5787"       transform="translate(75,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5789"       transform="translate(80,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5791"       transform="translate(85,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5793"       transform="translate(90,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5795"       transform="translate(95,0)"       xlink:href="#path4252"       y="0"       x="0" />    <use       height="100%"       width="100%"       id="use5797"       transform="translate(100,0)"       xlink:href="#path4252"       y="0"       x="0" />  </g>  <g     id="layer2"     transform="translate(5.2451471,8.9279683)"     style="display:inline;opacity:1">    <path       id="0"       d="m 4.7548529,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 9.7548529,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="1" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 14.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5000001,-5 2.5,5 0 0 1 2.5000001,-5 2.5,5 0 0 1 2.5,5 z"       id="2" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 19.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="3" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 24.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="4" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 29.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="5" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 34.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="6" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 39.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="7" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 44.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="8" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 49.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="9" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 54.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="10" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 59.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="11" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 64.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="12" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 69.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="13" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 74.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="14" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 79.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="15" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 84.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="16" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 89.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="17" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 94.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="18" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 99.754853,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.5,-5 2.5,5 0 0 1 2.5,-5 2.5,5 0 0 1 2.5,5 z"       id="19" />    <path       style="display:inline;opacity:0;fill:#000000;fill-opacity:1;stroke-width:1.20188606;stroke-miterlimit:4;stroke-dasharray:none"       d="m 104.75485,1.0720317 a 2.5,5 0 0 1 -2.5,5 2.5,5 0 0 1 -2.499997,-5 2.5,5 0 0 1 2.499997,-5 2.5,5 0 0 1 2.5,5 z"       id="20" />  </g>  <g     transform="translate(4.3500016,8.0625619)"     style="display:inline"     id="layer4">    <path       id="cross"       d="m 3.9843731,3.6481463 c -2.9687502,2.9375 -2.9687502,2.9375 -2.9687502,2.9375 l 1.5312503,-1.46875 -1.5000003,-1.53125 2.9687502,3"       style="display:inline;fill:none;stroke:#000000;stroke-width:0.60000002;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:none;stroke-opacity:1" />  </g></svg>';

    this.answerMap = {};
    var answerLayer = this.scaleImage.getElementById("layer2");
    var answerElements = answerLayer.getElementsByTagName("path"); //Attach event listener to clickable areas.
    for (var i = 0; i < answerElements.length; i++) {
        if (answerElements[i].id < 0 || answerElements[i].id > 20 || answerElements[i].id.length > 2) {
            TheFragebogen.logger.warn("QuestionnaireItemNASATLXScale._createAnswerNode()", "Invalid id (01 <= id <= 57): " + answerElements[i].id);
            continue;
        }
        this.answerMap[answerElements[i].id] = answerElements[i];
        answerElements[i].onclick = (this._handleChange).bind(this);
        TheFragebogen.logger.info("Attached Element No. ", answerElements[i].id + " at " + i);
    }


    // this.answerMap = {};
    // var answerElements = this.scaleImage.getElementsByTagName("path"); //Attach event listener to clickable areas.
    // for (var i = 0; i < answerElements.length; i++) {
    //     if (answerElements[i].id < 0 || answerElements[i].id > 20 || answerElements[i].id.length > 2) {
    //         continue;
    //     }
    //     this.answerMap[answerElements[i].id] = answerElements[i];
    //     answerElements[i].onclick = (this._handleChange).bind(this);
    //     TheFragebogen.logger.info("Attached Element No. ", answerElements[i].id + " at " + i);
    // }

    this.scaleImage.getElementById("labelLeft").textContent = this.captionLeft;
    this.scaleImage.getElementById("labelRight").textContent = this.captionRight;

    this.crossImage = this.scaleImage.getElementById("cross");
    //Problem identified here by the tests while using Safari 7.0.6 --- this.crossImage === null
    this.crossImage.setAttributeNS(null, "opacity", 0);

    if (this.answer != null) this._updateUI();

    node.appendChild(this.scaleImage);
    return node;
};
QuestionnaireItemNASATLXScale.prototype._handleChange = function (event) {
    if (!this.isEnabled()) return;

    this.setAnswer(event.target.id);

    TheFragebogen.logger.info("QuestionnaireItemNASATLXScale._handleChange()", this.getAnswer());

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
};
/**
 Called when the 7pt. Quality Scale UI should be updated.<br>
 Update the UI.
 */
QuestionnaireItemNASATLXScale.prototype._updateUI = function () {
    if (!this.isUIcreated()) return;

    if (this.answer == null) {
        this.crossImage.setAttributeNS(null, "opacity", 0);
        return;
    }
    if (this.answerMap[this.getAnswer()] == undefined) {
        TheFragebogen.logger.error("QuestionnaireItemNASATLXScale._updateUI()", "Invalid answer provided: " + this.getAnswer());
        return false;
    }

    //Displays cross
    this.crossImage.setAttributeNS(null, "opacity", 1);

    var bbx = this.answerMap[this.answer].getBBox();
    TheFragebogen.logger.info("QuestionnaireItemNASATLXScale._updateUI()", this.answer + ", " + this.answerMap[this.answer].id + " BBox: " + bbx.x + " " + bbx.y + " " + bbx.width + " " + bbx.height);
    this.crossImage.setAttributeNS(null, "transform", "translate(" + (bbx.x - bbx.width/2 + 3.3) + "," + (bbx.y + bbx.height/2 - 3) + ")");
    // var transform = this.answerMap[this.answer].getTransformToElement(this.crossImage.parentElement)
    // TheFragebogen.logger.info("QuestionnaireItemNASATLXScale._updateUI()", transform.e + " " + transform.f);
    // this.crossImage.setAttributeNS(null, "transform", "translate(" + (transform.e - this.crossImage.getBBox().width / 2 - 0.6) + "," + (transform.f - 2.0) + ")"); //Remark: 0.6 and 0.4 are magic.
};
QuestionnaireItemNASATLXScale.prototype.setAnswer = function (answer) {
    if (answer === null) {
        this.answer = null;
        this._updateUI();
        return true;
    }
    if (answer < 0 || answer > 20) {
        TheFragebogen.logger.error("QuestionnaireItemNASATLXScale.setAnswer()", "Invalid answer provided: " + answer);
        return false;
    }

    TheFragebogen.logger.info("QuestionnaireItemNASATLXScale.setAnswer()", answer);
    this.answer = answer;

    this.markRequired();
    this._sendReadyCallback();

    this._updateUI();
    return true;
};
QuestionnaireItemNASATLXScale.prototype.releaseUI = function () {
    this.node = null;
    this.uiCreated = false;

    this.scaleImage = null;
    this.answerMap = null;
    this.crossImage = null;
};
QuestionnaireItemNASATLXScale.prototype.getData = function () {
    return [this.getQuestion(), this.getAnswer()];
};
QuestionnaireItemNASATLXScale.prototype._checkData = function (data) {
    return data[0] == this.question;
};
QuestionnaireItemNASATLXScale.prototype.setData = function (data) {
    if (!this._checkData(data)) return false;

    this.setAnswer(data[1]);
    return true;
};
QuestionnaireItemNASATLXScale.prototype.getOptionList = function (data) {
    return "0-20";
};
