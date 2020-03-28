State: (@() {
    @Start(){};
    @Running(){};
    @End(){};
    return @() {
        Start; Running; End;
    };
})();

state: State.Start;
while (state != State.End) {
    println(match state {
        State.Start => (@() { state: State.Running; return "start"; })(),
        State.Running => (@() { state: State.End; return "running" })(),
    } else "end");
}